/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_PROTOCOL_PING_HPP
#define LIBBITCOIN_PROTOCOL_PING_HPP

#include <memory>
#include <system_error>
#include <boost/asio.hpp>
#include <boost/date_time.hpp>
#include <boost/system/error_code.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/network/channel.hpp>
#include <bitcoin/bitcoin/network/timeout.hpp>
#include <bitcoin/bitcoin/primitives.hpp>
#include <bitcoin/bitcoin/utility/dispatcher.hpp>
#include <bitcoin/bitcoin/utility/threadpool.hpp>

namespace libbitcoin {
namespace network {

class protocol_ping;
typedef std::shared_ptr<protocol_ping> protocol_ping_ptr;

class BC_API protocol_ping
  : public std::enable_shared_from_this<protocol_ping>
{
public:
    protocol_ping(channel_ptr node, threadpool& pool, const timeout& timeouts);

    /// This class is not copyable.
    protocol_ping(const protocol_ping&) = delete;
    void operator=(const protocol_ping&) = delete;

    void start();

private:
    bool stopped() const;
    void stop(const std::error_code& ec);

    void clear_timer();
    void reset_timer();
    void set_timer(const boost::posix_time::time_duration& timeout);
    void handle_timer(const boost::system::error_code& ec);

    void handle_send_ping(const std::error_code& ec) const;
    void handle_send_pong(const std::error_code& ec) const;
    void handle_receive_ping(const std::error_code& ec,
        const ping_type& ping);
    void handle_receive_pong(const std::error_code& ec,
        const pong_type& ping, uint64_t nonce);

    channel_ptr node_;
    const timeout& timeouts_;
    boost::asio::deadline_timer timer_;
    dispatcher dispatch_;
    bool stopped_;
};

} // namespace network
} // namespace libbitcoin

#endif
