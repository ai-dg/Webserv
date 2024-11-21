/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SessionManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dagudelo <dagudelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:56:10 by dagudelo          #+#    #+#             */
/*   Updated: 2024/11/21 20:18:31 by dagudelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../00-shared/includes.hpp"

class SessionManager
{
    private:
        std::map<std::string, std::map<std::string, std::string> > sessions;

        /**
         * @brief Private methods
         */
        std::string generateSessionsId();

    public:
        /**
         * @brief Copelien form
         */
        SessionManager();
        SessionManager(SessionManager const& src);
        SessionManager& operator=(SessionManager const& src);
        ~SessionManager();
        
        /**
         * @brief Public methods
         */
        std::string createSessions();
        bool sessionExist(std::string const& sessionId);
        std::map<std::string, std::string>& getSession(std::string const& sessionId);
        void saveSessionsToFile();
        void loadSessionsFromFile();
};

