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


// user agentlar omfg ekstra güvenlik
const std::vector<std::string> userAgents = {
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:97.0) Gecko/20100101 Firefox/97.0",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:97.0) Gecko/20100101 Firefox/97.0",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:97.0) Gecko/20100101 Firefox/97.0"
};
// Rastgele bir useragent seçiyoruz aşko
std::string getRandomUserAgent() {
    static std::mt19937 mt(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<size_t> dist(0, userAgents.size() - 1);
    return userAgents[dist(mt)];
}
// bitti userler amk


// header dosyasına eklenicek daha temiz olucak.
std::string solveCaptcha(const std::string& captchaSiteKey, const std::string& pageUrl, const std::string& apiKey) {
    std::string apiUrl = "http://2captcha.com/in.php?key=" + apiKey + "&method=userrecaptcha&googlekey=" + captchaSiteKey + "&pageurl=" + pageUrl;
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void* buffer, size_t size, size_t nmemb, void* userp) -> size_t {
            static_cast<std::string*>(userp)->append(static_cast<char*>(buffer), size * nmemb);
            return size * nmemb;
        });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return "";
    }

    if (response.find("OK|") == std::string::npos) {
        std::cerr << "Failed to submit captcha to 2Captcha: " << response << std::endl;
        return "";
    }

    std::string captchaId = response.substr(3);

    response.clear();
    apiUrl = "http://2captcha.com/res.php?key=" + apiKey + "&action=get&id=" + captchaId;
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5)); // 5 saniye bekleyin
        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void* buffer, size_t size, size_t nmemb, void* userp) -> size_t {
                static_cast<std::string*>(userp)->append(static_cast<char*>(buffer), size * nmemb);
                return size * nmemb;
            });
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
                continue;
            }
            curl_easy_cleanup(curl);
        } else {
            std::cerr << "Failed to initialize CURL" << std::endl;
            continue;
        }
        if (response.find("OK|") != std::string::npos) {
            break;
        } else if (response.find("CAPCHA_NOT_READY") != std::string::npos) {
            continue;
        } else {
            std::cerr << "Failed to get captcha result from 2Captcha: " << response << std::endl;
            return "";
        }
    }
    std::string captchaResult = response.substr(3);
    return captchaResult;
}

//random gen func
static std::mt19937 mt(std::random_device{}());
template<typename T>
T fastRandomElement(const std::vector<T>& vec) {
    static std::uniform_int_distribution<size_t> dist; 
    dist = std::uniform_int_distribution<size_t>(0, vec.size() - 1); 
    return vec[dist(mt)];
}


//curl needed this func <3
size_t CurlWrite_CallbackFunc_StdString(void* contents, size_t size, size_t nmemb, std::string* s) {//curl
    size_t newLength = size * nmemb;
    size_t oldLength = s->size();
    try { s->resize(oldLength + newLength); }
    catch (std::bad_alloc& e) { return 0; }

    std::copy((char*)contents, (char*)contents + newLength, s->begin() + oldLength);
    return size * nmemb;
}
std::string userAgent = getRandomUserAgent()
std::string cUrl(std::pair<string, string> proxy,std::string proxy_type,std::string target_url) {
    //keeped kaju
    //curl set options
    CURL* curl;
    CURLcode res;
    struct curl_slist* headers = NULL;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    std::string s;
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
        headers = curl_slist_append(headers, userAgent);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);//https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);//httpss

        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 8L);	   //reconnect timeout
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 8L);//connect timeout
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 8L);//receiving data timeout

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString); //idk (needed)

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);             // write result
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt"); // reads cookie file on site
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");  // reads cookie file on site
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);        // turn off follow location to server
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
       
           
        curl_easy_setopt(curl, CURLOPT_URL, target_url.c_str()); // target url

        // this there checks result codes

         /* ************************************************* */
        retr:s = "";
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {//Check for errors
            fprintf(stderr, "curl_easy_perform() failed: %d %s\n", res, curl_easy_strerror(res));
            system("pause");
            goto retr;
        }
        if (s.find("Attention Required") != std::string::npos || //ip banned by CloudFlare
            s.find("Access denied") != std::string::npos ||
            s.find("restrict access") != std::string::npos) {
            system("pause");
            goto retr;
        }
        else if (s.find("503 Service Temporarily Unavailable") != std::string::npos ||
            s.find("408 Request Time-out") != std::string::npos) {
            system("pause");
            goto retr;
        }
        /* ************************************************* */

        curl_easy_cleanup(curl);


        //clean memory
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    //global clean memory
    curl_global_cleanup();
    return s;

}

int main(void)
{   
 std::string r1 = cUrl(proxies[0], "http", "https://2captcha.com/demo/cloudflare-turnstile");
    //proxy path
    ifstream file("proxys.txt");
    std::string captchaSiteKey = ""; // CAPTCHA API KEY
    std::string pageUrl = r1; // target url
    std::string apiKey = "captcha"; // 2captcha api key

    // Captcha çözme işlemini gerçekleştir
    std::string captchaResult = solveCaptcha(captchaSiteKey, pageUrl, apiKey);

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
    retry:
   
    if (r1.find("Just a moment") != std::string::npos) {
        // Api needed for solve captcha
        Sleep(1000);// mimic waiting process
        // Solved capthca cookies to login
        goto retry;
    }
        
    std::cout << r1 << std::endl;
 
    return 0;
    std::cout << r1;
    



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

