#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

/// @brief Callback function for Download function
/// @param buffer 
/// @param itemsize 
/// @param nitems 
/// @param map 
/// @return 
size_t write_to_map(char *buffer, size_t itemsize, size_t nitems, FILE *map) {
    return fwrite(buffer, itemsize, nitems, map);
}

/// @brief Download map Function
/// @param url URL of server like http://example.com:2000
/// @param username Username
/// @param password Password
/// @param download_name Name of file to be saved with
/// @return 
int download_map(const char *url,const char *username,const char *password, const char *download_name) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "CURL INIT FAILED!\n");
        return EXIT_FAILURE;
    }

    // URL to request
    char *request_url;
    size_t size = 8 + strlen(url) + 14 + strlen(username) + strlen(password) + 1;
    request_url = (char *)malloc(size);
    if (request_url == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    sprintf(request_url, "%s/download_map/%s/%s", url, username,password);

    // Set curl options
    FILE *map = fopen(download_name, "w");
    curl_easy_setopt(curl, CURLOPT_URL, request_url);
    curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_map);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, map);

    // Send Request
    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        fprintf(stderr, "REQUEST ERROR: %s\n", curl_easy_strerror(result));
        fclose(map);
        free(request_url);
        curl_easy_cleanup(curl);
        return EXIT_FAILURE;
    }

    fclose(map);
    free(request_url);
    curl_easy_cleanup(curl);
    return EXIT_SUCCESS;
}

/// @brief Callback function for Upload function
/// @param buffer 
/// @param size 
/// @param nmemb 
/// @param userdata 
/// @return 
size_t read_callback(void *buffer, size_t size, size_t nmemb, void *userdata) {
    FILE *file = (FILE *)userdata;

    size_t bytesRead = fread(buffer, size, nmemb, file);

    return bytesRead;
}

/// @brief Upload map file to server function
/// @param username username
/// @param password Password
/// @param url URL of server like http://example.com:2000
/// @param filename File to be upload
/// @return 
int upload_file(const char *username,const char *password, const char *url, const char *filename) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "CURL INIT FAILED!\n");
        return EXIT_FAILURE;
    }
    // URL to request
    char *request_url;
    size_t size = 8 + strlen(url) + 14 + strlen(username) + strlen(password) + 1;
    request_url = (char *)malloc(size);
    if (request_url == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    sprintf(request_url, "%s/upload/%s/%s", url, username,password);
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open file for reading\n");
        curl_easy_cleanup(curl);
        return EXIT_FAILURE;
    }
    curl_easy_setopt(curl, CURLOPT_URL, request_url);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, file);
    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        fprintf(stderr, "UPLOAD ERROR: %s\n", curl_easy_strerror(result));
    }
    fclose(file);
    curl_easy_cleanup(curl);

    return (result == CURLE_OK) ? EXIT_SUCCESS : EXIT_FAILURE;
}