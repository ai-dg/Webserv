/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:58:11 by dagudelo          #+#    #+#             */
/*   Updated: 2024/12/16 10:32:25 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../00-headers/00-shared/includes.hpp"
#include "../00-headers/01-core/Server.hpp"
#include "../00-headers/01-core/Conf.hpp"
#include "../00-headers/01-core/SessionManager.hpp"
#include "../00-headers/01-core/Sockets.hpp"
#include "../00-headers/01-core/RarManager.hpp"
#include "../00-headers/01-core/SignalHandler.hpp"
#include "../00-headers/02-utils/signals.hpp"
#include "../00-headers/02-utils/cleanup.hpp"
#include "../00-headers/02-utils/files.hpp"
#include "../00-headers/02-utils/Log.hpp"

int main(int ac, char **av)
{
    setupSignalHandler();
    try
    {
        std::string path;
        std::vector<int> fd_sockets;
        std::vector<Conf *> Configs;
        std::vector<Server *> Servers;

        /**
         * @brief Conditions du path, si NULL, path par defaut
         */
        if (ac >= 2)
            path.assign(av[1]);
        else 
            path = "config/server.conf";

        if (!checkFormatOfConfig(path))
        {
            if (signalPipeFd[0] != -1)
            {
                ::close(signalPipeFd[0]);
                signalPipeFd[0] = -1;
            }
            if (signalPipeFd[1] != -1)
            {
                ::close(signalPipeFd[1]);
                signalPipeFd[1] = -1;
            }
            throw PathNotCorrectFormat();
        };
        /**
         * @brief Extraire les informations dans le path
         */
        get_all_server_conf(path, Configs);
        SessionManager sessionManager;
        if (start_all_servers(fd_sockets, Servers, Configs) == 1)
            return 1;    
        /**
         * @brief Gestion du trafic de requetes et reponses (fd du client et du serveur)
         */
        request_and_response_fd_manager(fd_sockets, Servers, sessionManager);

        Log::cleanup();
        clearMemory(Configs);
        clearArray(Servers);
        if (signalPipeFd[1] != -1)
        {
            ::close(signalPipeFd[1]);
            signalPipeFd[1] = -1;
        }
        Log::print_final_log("Clean", "exit");
    }
    catch (SignalException const& e)
    {
        std::cerr << "SignalException: " << e.what() << std::endl;
        Log::cleanup();
        return 1;
    }
    catch (std::exception const& e)
    {
        std::cerr << "General exception: " << e.what() << std::endl;
        Log::cleanup();
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception!" << std::endl;
        Log::cleanup();
        return 1;
    }
    return (0);
}
