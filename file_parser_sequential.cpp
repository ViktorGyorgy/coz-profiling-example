#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <string>

bool cmp_reverse(std::pair<std::string, int>& a, std::pair<std::string, int>& b) 
{ 
    return a.second > b.second; 
} 


std::vector<std::pair<std::string, int>> ten_most_popular_static_images
(std::map<std::string, int>& incidence_map)
{
    std::vector<std::pair<std::string, int>> sorted_incidence;
    for(auto& p : incidence_map)
    {
        sorted_incidence.push_back(p);
    }   

    std::sort(sorted_incidence.begin(), sorted_incidence.end(), cmp_reverse);
    sorted_incidence.resize(10);
    return sorted_incidence;
} 


int main()
{
    std::ifstream ifs("access.log", std::ifstream::in);
    std::string line;

    std::map<std::string, int> incidence_map;
    std::regex image_path_regex("GET (/static/image[^\\s]+)");

    while(std::getline(ifs, line))
    {
        std::regex_iterator image_path_begin = std::sregex_iterator(line.begin(), line.end(), image_path_regex);
        std::regex_iterator image_path_end = std::sregex_iterator();

        for(std::regex_iterator it = image_path_begin; it != image_path_end; it++)
        {
            std::string path_to_file = it->str(1);
            
            if(!incidence_map.contains(path_to_file))
            {
                incidence_map[path_to_file] = 0;
            }
            incidence_map[path_to_file]++;
        }
    };
    ifs.close();

    std::vector<std::pair<std::string, int>> incidence_sorted = ten_most_popular_static_images(incidence_map);
    std::ofstream ofs("results_sequential.txt");
    for(auto it= incidence_sorted.begin(); it != incidence_sorted.end(); it++)
    {
        ofs << it->first << " " << it->second << std::endl;
    }

    ofs.close();
    return 0;
}