/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_IOMX_H_

#define ANDROID_IOMX_H_

#include <binder/IInterface.h>
#include <utils/List.h>
#include <utils/String8.h>

#include <OMX_Core.h>

#define IOMX_USES_SOCKETS       0

namespace android {

class IMemory;
class IOMXObserver;

class IOMX : public IInterface {
public:
    DECLARE_META_INTERFACE(OMX);

    typedef void *buffer_id;
    typedef void *node_id;

#if IOMX_USES_SOCKETS
    // If successful, returns a socket descriptor used for further
    // communication. Caller assumes ownership of "*sd".
    virtual status_t connect(int *sd) = 0;
#endif

    virtual status_t list_nodes(List<String8> *list) = 0;

    virtual status_t allocate_node(const char *name, node_id *node) = 0;
    virtual status_t free_node(node_id node) = 0;

    virtual status_t send_command(
            node_id node, OMX_COMMANDTYPE cmd, OMX_S32 param) = 0;

    virtual status_t get_parameter(
            node_id node, OMX_INDEXTYPE index,
            void *params, size_t size) = 0;

    virtual status_t set_parameter(
            node_id node, OMX_INDEXTYPE index,
            const void *params, size_t size) = 0;

    virtual status_t use_buffer(
            node_id node, OMX_U32 port_index, const sp<IMemory> &params,
            buffer_id *buffer) = 0;

    virtual status_t allocate_buffer(
            node_id node, OMX_U32 port_index, size_t size,
            buffer_id *buffer) = 0;

    virtual status_t allocate_buffer_with_backup(
            node_id node, OMX_U32 port_index, const sp<IMemory> &params,
            buffer_id *buffer) = 0;

    virtual status_t free_buffer(
            node_id node, OMX_U32 port_index, buffer_id buffer) = 0;

#if !IOMX_USES_SOCKETS
    virtual status_t observe_node(
            node_id node, const sp<IOMXObserver> &observer) = 0;

    virtual void fill_buffer(node_id node, buffer_id buffer) = 0;

    virtual void empty_buffer(
            node_id node,
            buffer_id buffer,
            OMX_U32 range_offset, OMX_U32 range_length,
            OMX_U32 flags, OMX_TICKS timestamp) = 0;
#endif
};

struct omx_message {
    enum {
        EVENT,
        EMPTY_BUFFER_DONE,
        FILL_BUFFER_DONE,

#if IOMX_USES_SOCKETS
        EMPTY_BUFFER,
        FILL_BUFFER,
        SEND_COMMAND,
        DISCONNECT,
        DISCONNECTED,
#endif

        // reserved for OMXDecoder use.
        START,
        INITIAL_FILL_BUFFER,

        // reserved for OMXObserver use.
        QUIT_OBSERVER,
    } type;

    union {
        // if type == EVENT
        struct {
            IOMX::node_id node;
            OMX_EVENTTYPE event;
            OMX_U32 data1;
            OMX_U32 data2;
        } event_data;

        // if type == EMPTY_BUFFER_DONE || type == FILL_BUFFER
        //    || type == INITIAL_FILL_BUFFER
        struct {
            IOMX::node_id node;
            IOMX::buffer_id buffer;
        } buffer_data;

        // if type == EMPTY_BUFFER || type == FILL_BUFFER_DONE
        struct {
            IOMX::node_id node;
            IOMX::buffer_id buffer;
            OMX_U32 range_offset;
            OMX_U32 range_length;
            OMX_U32 flags;
            OMX_TICKS timestamp;
            OMX_PTR platform_private;  // ignored if type == EMPTY_BUFFER
        } extended_buffer_data;

        // if type == SEND_COMMAND
        struct {
            IOMX::node_id node;
            OMX_COMMANDTYPE cmd;
            OMX_S32 param;
        } send_command_data;

    } u;
};

class IOMXObserver : public IInterface {
public:
    DECLARE_META_INTERFACE(OMXObserver);

    virtual void on_message(const omx_message &msg) = 0;
};

////////////////////////////////////////////////////////////////////////////////

class BnOMX : public BnInterface<IOMX> {
public:
    virtual status_t onTransact(
            uint32_t code, const Parcel &data, Parcel *reply,
            uint32_t flags = 0);
};

class BnOMXObserver : public BnInterface<IOMXObserver> {
public:
    virtual status_t onTransact(
            uint32_t code, const Parcel &data, Parcel *reply,
            uint32_t flags = 0);
};

}  // namespace android

#endif  // ANDROID_IOMX_H_
