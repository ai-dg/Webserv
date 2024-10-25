 #include "../headers/ipTools.hpp"
 
uint32_t ipstoi(std::string ip_string)
 {
    struct in_addr ip_addr;
    memset(&ip_addr, 0, sizeof(ip_addr));
    if (inet_pton(AF_INET, ip_string.c_str(), &ip_addr) <= 0)
    {
        perror("inet_pton échoué");
        return -1;
    }
    return (int)ntohl(ip_addr.s_addr);
 }
 
