/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: subpark <subpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 22:37:54 by siun              #+#    #+#             */
/*   Updated: 2024/06/12 13:20:33 by subpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSE_HPP
# define CONFIGPARSE_HPP
# include <string>
# include <cstring>
# include <exception>
# include <vector>
# include <iostream>
# include <fstream>
# include <iterator>
# include <exception>
# include <algorithm>
# include <sstream>
# include <map>
#include "../CommonIncludes.hpp"


t_server												parseServerConfig(const std::vector<std::pair<std::string, int> >chunck);
std::string												openFile(std::string path);
std::vector<std::pair<std::string, int> >				findIndent(std::string str);
std::vector<std::vector<std::pair<std::string, int> > >	findChunck(std::vector<std::pair<std::string, int> > indents, std::string keyword);
std::vector<t_server>									configParse(std::string configFilePath);
std::string	parseString(const std::vector<std::pair<std::string, int> > chunck, std::string keyword);
std::map<std::string, location> parseLocations(std::vector<std::pair<std::string, int> > chunck);

# endif
