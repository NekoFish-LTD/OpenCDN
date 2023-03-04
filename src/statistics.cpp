#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

void statistics() {
    std::ifstream log_file("/etc/nginx/logs/access.log");
    std::string line;
    std::map<std::string, int> url_counts;

    while (std::getline(log_file, line)) {
        // 解析日志行，获取请求URL
        std::stringstream ss(line);
        std::string remote_addr, remote_user, time_local, request, status, body_bytes_sent, http_referer, http_user_agent;
        ss >> remote_addr >> remote_user >> time_local >> request >> status >> body_bytes_sent >> http_referer >> http_user_agent;

        // 提取URL
        std::string url;
        if (request.find("GET ") == 0) {
            url = request.substr(4);
            url = url.substr(0, url.find(' '));
        } else if (request.find("POST ") == 0) {
            url = request.substr(5);
            url = url.substr(0, url.find(' '));
        } else {
            continue; // 不处理其他请求类型
        }

        // 统计URL的访问次数
        url_counts[url]++;
    }

    // 输出每个URL的访问次数
    for (auto& item : url_counts) {
        std::cout << item.first << ": " << item.second << std::endl;
    }

    return 0;
}