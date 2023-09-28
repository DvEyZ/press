#include "Connection.h"
#include "utils/ResponseBuilder.h"

using namespace press;

void Connection::read() {
    auto self = shared_from_this();
    self->socket.async_read_some(
        asio::buffer(self->chunk.end() - Connection::CHUNK_SIZE, Connection::CHUNK_SIZE), 
        [self] (const std::error_code e, size_t bytes_transferred) {
            self->on_read(e, bytes_transferred);
        }
    );
}

void Connection::on_read(const std::error_code e, size_t bytes_transferred) {
    if(e) {
        this->shutdown();
        return;
    }

    auto sv = std::string_view(this->chunk.begin(), bytes_transferred);
    this->buffer.append(sv);

    if(this->wait_for == WaitFor::BodyWithSize) {
        for(unsigned int i = 0; i < this->buffer.length(); i++) {
            this->request.body.push_back(this->buffer[i]);
            this->body_transferred++;
            if(this->body_transferred > this->body_size) {
                // body read, respond and return
                this->respond();
                return;
            }
        }
        this->buffer.erase();
    }
    else if(this->wait_for == WaitFor::BodyChunked) {
        // UNIMPLEMENTED
    }
    else {
        size_t pos;
        while((pos = buffer.find("\r\n")) != std::string::npos) {
            std::string line = buffer.substr(0, pos);
            buffer.erase(0, pos + 2); // waste the \n
            
            if(this->wait_for == WaitFor::Request) {
                try {
                    this->request.add_first_line(line);
                } catch(http::ParseError& e) {
                    this->respond_error(e, http::Status::BadRequest);
                    return;
                };
                this->wait_for = WaitFor::Headers;
            } else if(this->wait_for == WaitFor::Headers) {
                if(line.empty()) {
                    if(this->request.headers.content_length.has_value()) {
                        this->wait_for = WaitFor::BodyWithSize;
                        this->body_size = this->request.headers.content_length.value();
                        this->body_transferred = 1;
                        
                        // Read pending buffer data
                        for(unsigned int i = 0; i < this->buffer.length(); i++) {
                            this->request.body.push_back(this->buffer[i]);
                            this->body_transferred++;
                            if(this->body_transferred > this->body_size) {
                                // body read, respond and return
                                this->respond();
                                return;
                            }
                        }
                        this->buffer.erase();
                    } else {
                        // body read, respond and return
                        this->respond();
                        return;
                    }
                } else {
                    try {
                        this->request.add_header_line(line);
                    } catch(http::ParseError& e) {
                        this->respond_error(e, http::Status::BadRequest);
                        return;
                    };
                }
            }
        }
    }
    
    this->read();
}

void Connection::respond() {
    http::Response res = this->on_recv(std::move(this->request));
    this->response_transfer = 0;
    this->response_buffer = res.to_string();
    this->write();
}


void Connection::respond_error(std::exception& e, http::Status status) {
    http::Response res = this->on_error(e, status);
    this->response_transfer = 0;
    this->response_buffer = res.to_string();
    this->write();
}

void Connection::write() {
    auto self = shared_from_this();
    this->socket.async_write_some(
        asio::buffer(
            this->response_buffer.begin().base() + this->response_transfer, 
            this->response_buffer.length() - this->response_transfer
        ),
        [self] (const asio::error_code e, size_t bytes_transferred) {
            self->on_write(e, bytes_transferred);
        }
    );
}

void Connection::on_write(const asio::error_code e, size_t bytes_transferred) {
    if(e) {
        this->shutdown();
    }

    this->response_transfer += bytes_transferred;

    if(static_cast<unsigned int>(this->response_transfer) < this->response_buffer.length()) {
        this->write();
    }
}

void Connection::shutdown() {
    if(socket.is_open()) {
        try {
            socket.shutdown(asio::socket_base::shutdown_type::shutdown_both);
        } catch(std::system_error& e) {}
    }
    socket.close();
}

void Connection::run() {
    this->read();
}
