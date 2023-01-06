/*
 * The MIT License (MIT)
 *
 * Copyright © 2015-2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "config.h"
#include <assert.h>
#include <errno.h>
#include NN_H
#include NN_REQREP_H
#include <string.h>

#include "err.h"
#include "compat/process.h"

#define CRI_PROTO_TIMEOUT (60 * 1000)

static void set_nn_socket_timeout(int sock)
{
    int timeout = CRI_PROTO_TIMEOUT;
    nn_setsockopt(sock, NN_SOL_SOCKET, NN_SNDTIMEO, &timeout, sizeof(timeout));
    nn_setsockopt(sock, NN_SOL_SOCKET, NN_RCVTIMEO, &timeout, sizeof(timeout));
}

int cri_proto_bind(const char *url)
{
    int sock = nn_socket(AF_SP, NN_REP);

    if (sock < 0)
        return -1;

    set_nn_socket_timeout(sock);

    if (nn_bind(sock, url) < 0)
        goto error;

    return sock;

error: {}
    errno_ignore(nn_close(sock));
    return -1;
}

int cri_proto_connect(const char *url)
{
    int sock = nn_socket(AF_SP, NN_REQ);

    if (sock < 0)
        return -1;

    set_nn_socket_timeout(sock);

    if (nn_connect(sock, url) < 0)
        goto error;

    return sock;

error: {}
    errno_ignore(nn_close(sock));
    return -1;
}

void cri_proto_close(int sock)
{
    nn_close(sock);
}
