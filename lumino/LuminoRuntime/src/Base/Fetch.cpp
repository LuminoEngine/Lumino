
#include "Internal.hpp"
#if 0
#ifdef LN_EMSCRIPTEN
#else
#include <curl/curl.h>
#endif
#include <LuminoEngine/Base/Fetch.hpp>

namespace ln {

class FetchGet {
};

namespace detail {

#ifdef LN_EMSCRIPTEN
#else
class CurlFetchContext {
public:
    FetchResponse response;
    std::string headers;

    Result<> getSync(StringView url) {
        const std::string actualURL = url.toUtf8();

        CURL* curl;
        CURLcode res;
        curl = curl_easy_init();

        curl_easy_setopt(curl, CURLOPT_URL, actualURL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onReceiveBody);
        curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, onReceiveHeader);
        //curl_easy_setopt(curl, CURLOPT_USER, this);

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        auto bb = std::string(response.body.begin(), response.body.end());

        response.url = url;
        //response.body = fetchResponse;
        if (res != CURLE_OK) {
            return err();
            //raiseCurlCode(res);
        }
        else {
            int response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

            response.status = response_code;
            //response.headers = parseHeaders(headers);
        }
        return ok();
    }

    static size_t onReceiveHeader(char* buffer, size_t size, size_t nitems, void* userdata) {
        auto* self = reinterpret_cast<CurlFetchContext*>(userdata);

        size_t bytes = size * nitems;
        self->headers.reserve(bytes);
        for (size_t c = 0; c < bytes; c++) {
            self->headers.push_back(buffer[c]);
        }
        return nitems * size;
    }

    static size_t onReceiveBody(char* ptr, size_t size, size_t nmemb, void* userdata) {
        auto* self = reinterpret_cast<CurlFetchContext*>(userdata);
        const size_t sizes = size * nmemb;
        self->response.body.insert(self->response.body.end(), (char*)ptr, (char*)ptr + sizes);
        // streamはCURLOPT_WRITEDATAで指定したバッファへのポインタ
        // std::vector<char>* recvBuffer = (std::vector<char>*)stream;
        // 
        // recvBuffer->
        return sizes;
    }
};
#endif

Result<> FetchManager::initialize() {
#ifdef LN_EMSCRIPTEN
#else
    if (curl_global_init(CURL_GLOBAL_ALL) != 0) {
        LN_ERROR("curl_global_init failed.");
        return err();
    }
#endif
    return ok();
}

void FetchManager::terminate() {
#ifdef LN_EMSCRIPTEN
#else
    curl_global_cleanup();
#endif
}

FetchResponse FetchManager::getSync(StringView url) {
#ifdef LN_EMSCRIPTEN
    return {};
#else
    CurlFetchContext context;
    context.getSync(url);
    return std::move(context.response);
#endif
}

} // namespace detail
} // namespace ln

#endif
