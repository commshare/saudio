/**
 * Copyright (c) 2011-2014, Vasileios Daras. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */

#ifndef AUDIO_STREAMING_SERVER_H
#define AUDIO_STREAMING_SERVER_H

#include "AudioLibrary.h"

//standard library includes
#include <fstream>
#include <map>

#include "http/Http.h"
#include "system/network/Server.h" 

class HttpResponse;
class IHttpRequestExtractor;
class IURLCodec;

class AudioStreamingServer : public Server {
    
    static std::map<std::string, HttpContentType> EXTENSION_TO_CONTENT_TYPE;

	std::unique_ptr<AudioLibrary> audioLibrary;

    std::unique_ptr<IHttpRequestExtractor> httpExtractor;       //object used to isolate the different parts of an HTTP request

    std::unique_ptr<IURLCodec> urlCodec;    //object used to encode/decode URLs

    public:

    /**
     * Constructor. Initializes the server by binding the
     * port passed as a parameter.
     *
     * @param port
     *        Port that the server listens to for incoming connections.
     *
     * @param maxConnections 
     *        Maximum connections the server can accept simultaneously.
     *
     * @param servingThreads
     *        How many threads the server will spawn.
     *
     * @param libraryDirectory
     *        Server's audio library root directory.
     */
    AudioStreamingServer(unsigned short port, unsigned maxConnections, unsigned servingThreads, const std::string& libraryDirectory);

    /**
     * Destructor.
     */
    virtual ~AudioStreamingServer();

    /**
     * Serves the client.
     */
    virtual void Serve(const std::shared_ptr<ISocket>& clientSocket) override;


    private:

    /**
     * Sends an HttpResponse header to the client.
     *
     * @param clientSocket
     *        Client's socket.
     *
     * @param response
     *        Http response to get header from.
     */
    void SendResponseHeader(const std::shared_ptr<ISocket>& clientSocket, const HttpResponse& response) const;

    
    /**
     * Streams a file to a client.
     *
     * @param clientSocket
     *        Client's socket to stream the file over.
     *
     * @param contentType
     *        Equivalent content type of the file.       
     *
     * @param file
     *        File to stream.
     */
    void StreamFile(const std::shared_ptr<ISocket>& clientSocket, std::ifstream& file, HttpContentType contentType) const;


    /**
     * Sends an M3U media list to the client.
     *
     * @param clientSocket
     *        Client's socket to send the list over.
     *
     * @param files
     *        List of files to populate the M3U list with.
     *
     * @param hostname
     *        Name of the host to create URLs in M3U for.
     */
    void SendMediaList(const std::shared_ptr<ISocket>& clientSocket, const std::string& keyword, const std::string& hostname) const;
};

#endif