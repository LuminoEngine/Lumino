
#pragma once

#if 0

namespace ln {

class Fetch {

};

//Response fetch(string url) {
//    return get::get(url);
//}
//
//Response fetch(const ln::String& url, const ln::String& method, Array<Param<string>> params) {
//    if (method == "GET") {
//        return get::get(url);
//    }
//    else if (method == "POST") {
//        return post::post(url, params);
//    }
//    else {
//        return get::get(url);
//    }
//}

namespace detail {

class FetchResponse {
public:
    std::vector<uint8_t> body;
    int status;
    bool ok() {
        return this->status == 200;
    }
    String url;
    String statusText;
    Array<std::pair<String, String>> headers;
    //string text() {
    //    // TODO: parse body into text.
    //    return this->body;
    //};
    //string json() {
    //    // b = this -> body;
    //    // return parseJSON(b);
    //    return "";
    //};
};


class FetchManager {
public:
    static Result<> initialize();
    static void terminate();
    static FetchResponse getSync(StringView url);
};
} // namespace detail
} // namespace ln

#endif

