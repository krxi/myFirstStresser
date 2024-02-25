#include <iostream>
#include <curl/curl.h>
#include <thread> 
#include <chrono>
#include <vector>
#include <chrono>
#include <random>
#include <iostream>
#include <stdexcept>
#include <fstream>

using namespace std;


//random gen func
static std::mt19937 mt(std::random_device{}());
template<typename T>
T fastRandomElement(const std::vector<T>& vec) {
    static std::uniform_int_distribution<size_t> dist; 
    dist = std::uniform_int_distribution<size_t>(0, vec.size() - 1); 
    return vec[dist(mt)];
}


//curl needed this func <3
size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
    //helps to make write
}


void cUrl(std::pair<string, string> proxy,std::string proxy_type,std::string target_url) {
    //keeped kaju
    //curl set options
    CURL* curl;
    CURLcode res;
    struct curl_slist* headers = NULL;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        //headers

        //if u find a good agent, add to list.
        headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8");
        headers = curl_slist_append(headers, "Accept-Language: tr-TR,tr;q=0.9,en-US;q=0.8,en;q=0.7");
        headers = curl_slist_append(headers, "Cache-Control: max-age=0");
        headers = curl_slist_append(headers, "Connection: keep-alive");
        headers = curl_slist_append(headers, "If-Modified-Since: Thu, 17 Oct 2019 07:18 : 26 GMT");
        headers = curl_slist_append(headers, "If-None-Match: \"3147526947+gzip\" ");
        headers = curl_slist_append(headers, "Sec-Ch-Ua-Platform:\"Windows\"");
        headers = curl_slist_append(headers, "Sec-Fetch-Dest: document");
        headers = curl_slist_append(headers, "Sec-Fetch-Mode: navigate");
        headers = curl_slist_append(headers, "Sec-Fetch-Site: none");
        headers = curl_slist_append(headers, "Sec-Fetch-User: ?1");
        headers = curl_slist_append(headers, "TE: trailers");
        headers = curl_slist_append(headers, "Sec-Gpc: 1");
        headers = curl_slist_append(headers, "DNT: 1");
        headers = curl_slist_append(headers, "Upgrade-Insecure-Requests:: 1");
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36");

        //proxy settings
        std::string proxy_url = proxy_type + "://" + proxy.first;
        std::string userpass = proxy.second;

        //proxy ip:port
        curl_easy_setopt(curl, CURLOPT_PROXY, proxy_url.c_str());
        //proxy add auth 
        curl_easy_setopt(curl, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
        //proxy set user:pass
        curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, userpass.c_str());

        //set headers (do not touch)
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
       
            //Enable TCP keepalive option
            curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

            //Set keepalive time to 120s
            curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, 120L);

            //interval time between keep-alive probes 60s
            curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, 60L);
            curl_easy_setopt(curl, CURLOPT_URL, target_url.c_str());

            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        //clean memory
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    //global clean memory
    curl_global_cleanup();

}

int main(void)
{   

    //proxy path
    ifstream file("proxys.txt");

    //proxy.txt per line
    string line;

    //IMPORTANT proxies vec
    std::vector<std::pair<string, string>> proxies;

    std::cout << "Starting reading proxies..." << endl;

    //streams proxys.txt
    while (getline(file, line)) {

        //temporary token for parse line
        std::string token;
        //streams line
        std::stringstream ss(line);
        //temporary parsed string (4 value)
        std::vector<string> temporary;
        //loop for token
        while (std::getline(ss, token, ':')) {
            temporary.push_back(token);
        }

        //to check values it is empty or not
        if (temporary[0].empty() || temporary[1].empty() || temporary[2].empty() || temporary[3].empty()) {
            std::cout << "Err to add vector. Please fix your shit txt file bro XXAXAXAXAX" << endl;

        }
        else {
            // Writing values to vector
            string ipport = temporary[0] + ":" + temporary[1];
            string userpass = temporary[2] + ":" + temporary[3];

            //Adding values to vector
            proxies.push_back(std::pair(ipport, userpass));

            //Clear temporary vec
            temporary.clear();
        }
    }
  
    std::cout << "Done !" << endl;
    //test func
    cUrl(proxies[0],"http","https://api.ipify.org");
    
    /* ************************************************* */
    /*
    * 
    * This for loop needed maybe in the future...
    * 
    for (int i = 0; i < proxies.size(); i++) {
        std::pair a = proxies[i];
        //auto  a = fastRandomElement(numbers);
        //std::cout << a.first << " " << a.second << endl;
    }



   
    const int numberOfThreads = 1000;
    std::vector<std::thread> threads;

    for (int i = 0; i < numberOfThreads; ++i) {
       threads.push_back(std::thread(foo));
    }


    for (auto& th : threads) {
       th.join();
    }

  */
   /* ************************************************* */
  
}

