#include "../headers/HttpRequest.hpp"

HttpRequest::HttpRequest(std::string req)
{
    std::cout << req << std::endl;
}

HttpRequest::~HttpRequest()
{    
    std::cout << "end req" << std::endl;
}