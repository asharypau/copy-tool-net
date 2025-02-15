#ifndef CLIENT_MESSAGE_QUEUE_HANDLER_H
#define CLIENT_MESSAGE_QUEUE_HANDLER_H

#include <mutex>
#include <queue>
#include <vector>

#include "../models/Message.h"
#include "FileClient.h"

namespace Client
{
    class MessagesQueueHandler
    {
    public:
        explicit MessagesQueueHandler(boost::asio::ip::tcp::socket& socket);

        /**
         * @brief Handles a batch of messages by adding them to the queue and initiating reading/writing processes.
         *
         * @param messages A vector of `Message` objects to be processed. Each message is moved into the internal message queue for further handling.
         *
         * The function operates as follows:
         * - Acquires a lock on the mutex (`_mtx`) to ensure thread safety.
         * - Moves each message from the input vector into the internal message queue.
         * - If a write operation is not in progress, it starts writing the first message in the queue.
         * - If a read operation is not in progress, it initiates the read process.
         */
        void handle(std::vector<Message>& messages);

    private:
        /**
         * @brief Handles the completion of a write operation by processing the next message in the queue.
         *
         * The function operates as follows:
         * - Acquires a lock on the mutex (`_mtx`) to ensure thread safety.
         * - Moves the front message from the message queue to the pending messages list.
         * - Sets `_writing_in_progress` to `false` to indicate the completion of the current write operation.
         * - If there are more messages in the queue, it starts writing the next message.
         */
        void write_handle();

        /**
         * @brief Handles the confirmation of a successfully received message by removing it from the pending queue.
         *
         * The function operates as follows:
         * - Acquires a lock on the mutex (`_mtx`) to ensure thread safety.
         * - Searches for the message in the queue using the provided `id`.
         * - If the message is found, removes the message from the queue.
         * - Calls the `read` method to continue processing subsequent messages.
         *
         * @param id The Id of the message to be removed from the pending messages queue.
         */
        void read_handle(Tcp::header_t confirmation_id);

        std::queue<Message> _messages;
        std::vector<Message> _pending_messages;
        FileClient _file_client;
        std::mutex _mtx;
        bool _writing_in_progress;
        bool _reading_in_progress;
    };
}

#endif  // CLIENT_MESSAGE_QUEUE_HANDLER_H
