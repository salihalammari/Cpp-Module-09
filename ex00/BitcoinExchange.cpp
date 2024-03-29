/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slammari <slammari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:07:07 by slammari          #+#    #+#             */
/*   Updated: 2023/04/17 05:23:57 by slammari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

std::pair<std::string, float> parse_data(std::string data)
{
	std::map<std::string, float> data_betcoin;
	std::string date = data.substr(0, data.find(','));
	std::string value = data.substr(data.find(',') + 1, data.length());
	data_betcoin[date] = std::atof(value.c_str());
	return make_pair(date, std::atof(value.c_str()));
}

bool isValidDate(std::string str)
{
	std::string year, month, day;
	std::string date = str;
	year = str.substr(0, str.find('-'));
	month = str.substr(str.find('-') + 1, 2);
	day = str.substr(str.find_last_of('-') + 1, 2);
	if (year.size() != 4)
		return (std::cout << "Error: bad input => " << date << std::endl, 1);
	for(size_t i = 0; i < year.size(); i++)
	{
		if ((i < 2 && (!isdigit(day[i]) || !isdigit(month[i]))) || !isdigit(year[i]))
		{
			std::cout << "Error: bad input => " << date << std::endl;
			return 1;
		}
	}
	return 0;
}

bool isValidPrice(std::string price)
{
	int Dot = 0;
	
	for(size_t i = 0; i < price.size(); i++)
	{
		if (i == 0 && (price[i] == '-' || price[i] == '+'));
		else if(!isdigit(price[i]) && price[i] != '.') {
			std::cout << "Error: bad input => " << price << std::endl;
			return 1;
		}
		if (price[i] == '.')
			Dot++;
	}
	if (Dot > 1)
	{
			std::cout << "Error: bad input => " << price << std::endl;
			return 1;
	}
	return 0;
}

std::pair<std::string, float> parse_input(std::string data)
{
	std::map<std::string, float> data_betcoin;
	std::string date = data.substr(0, 10);
	std::string bet = data.substr(10,data.find(" | ") - 3);
	bet = bet.substr(0,bet.find(" | "));
	if(!bet.empty())
	{
		std::cout << "Error: bad input => " << data << std::endl;
		return (std::make_pair("error",0));
	}
	if (isValidDate(date))
		return (std::make_pair("error",0));
	if (data.find("|") == std::string::npos)
		return (std::make_pair(data,0));
	std::string value = data.substr(data.find(" | ") + 3);
	if(value.empty())
		std::cout << value << std::endl;
	if (isValidPrice(value))
		return (std::make_pair("error",0));
	data_betcoin[date] = std::atof(value.c_str());
	return std::make_pair(date, std::atof(value.c_str()));
}

void    read_map(std::map<std::string, float> &data_betcoin)
{
	std::string line;
	std::ifstream namefile;
	namefile.open("data.csv");
	if (namefile.is_open())
	{
		while (!namefile.eof())
		{
			std::getline(namefile, line);
			if (line != "date,exchange_rate")
			{
				data_betcoin.insert(parse_data(line));
			}
		}
	}
	else
		std::cout << "Error: could not open file." << std::endl;
}

bool check_input(std::string str){

	int year, month, day;
	std::string date = str;
	year = std::atoi(str.substr(0, str.find('-')).c_str());
	month = std::atoi(str.substr(str.find('-') + 1, 2).c_str());
	day = std::atoi(str.substr(str.find_last_of('-') + 1, 2).c_str());
	if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31){
		std::cout << "Error: bad input => " << date << std::endl;
		return 1;
	}
	if (month == 2) {
		if (day > 28 + (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
			std::cout << "Error: bad input => " << date << std::endl;
			return 1;
	}
	} else if (month == 4 || month == 6 || month == 9 || month == 11) {
		if (day > 30) {
			std::cout << "Error: bad input => " << date << std::endl;
			return 1;
	}
	} else {
	if (day > 31) {
		std::cout << "Error: bad input => " << date << std::endl;
		return 1;
	}
	}
	return 0;
}

float get_exchange(std::string &date, std::map<std::string, float> &data_betcoin)
{
	std::map<std::string, float>::iterator it = data_betcoin.lower_bound(date);
	if (it == data_betcoin.begin() && it->first != date)
		return 0;
	if (it != data_betcoin.begin() && it->first != date )
		it--;
	
	return it->second;
}


void parser(const char *name)
{
	std::ifstream namefile(name);
	std::string line;
	
	std::map<std::string, float> data_betcoin;
	std::pair<std::string, float> input;
	read_map(data_betcoin);
	if (namefile.is_open())
	{
		int i = 0;
		
		while (!namefile.eof())
		{
			std::getline(namefile, line);
			if(i == 0)
			{
				if(line != "date | value"){
					std::cout << "Error: head empty" << line << std::endl;
					return ;
				}
				i++;	
			}
			if (line != "date | value" && !line.empty())
			{
				input = parse_input(line);
				if (input.first == "error" || check_input(input.first) )
					continue;
				if (input.second >= 1000)
					std::cout << "Error: too large a number" << std::endl;
				else if (input.second < 0)
					std::cout << "Error: not a positive number." << std::endl;
				else
				{
					float exchange = get_exchange(input.first, data_betcoin);
					std::cout << input.first << " => "<< input.second << " = " << input.second * exchange << std::endl;
				}
			}
		}
	}
	else
		std::cout << "Error: could not open file." << std::endl;	
}
