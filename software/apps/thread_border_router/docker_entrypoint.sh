#!/bin/bash
#
#  Copyright (c) 2018, The OpenThread Authors.
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  3. Neither the name of the copyright holder nor the
#     names of its contributors may be used to endorse or promote products
#     derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#

export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

function parse_args()
{
    while [ $# -gt 0 ]; do
        case $1 in
            --radio-url)
                RADIO_URL="$2"
                shift
                shift
                ;;
            --interface | -I)
                TUN_INTERFACE_NAME=$2
                shift
                shift
                ;;
            --backbone-interface | -B)
                BACKBONE_INTERFACE_ARG="-B $2"
                shift
                shift
                ;;
            --nat64-prefix)
                NAT64_PREFIX=$2
                shift
                shift
                ;;
            --disable-default-prefix-route)
                AUTO_PREFIX_ROUTE=false
                shift
                ;;
            --disable-default-prefix-slaac)
                AUTO_PREFIX_SLAAC=false
                shift
                ;;
            *)
                shift
                ;;
        esac
    done
}

parse_args "$@"

[ -n "$RADIO_URL" ] || RADIO_URL="spinel+hdlc+uart:///dev/ttyACM0"
[ -n "$TUN_INTERFACE_NAME" ] || TUN_INTERFACE_NAME="wpan0"
[ -n "$AUTO_PREFIX_ROUTE" ] || AUTO_PREFIX_ROUTE=true
[ -n "$AUTO_PREFIX_SLAAC" ] || AUTO_PREFIX_SLAAC=true

# We can't use 64:ff9b:: if connecting to a local network
#[ -n "$NAT64_PREFIX" ] || NAT64_PREFIX="64:ff9b::/96"
[ -n "$NAT64_PREFIX" ] || NAT64_PREFIX="2001:db8:1:ffff::/96"


echo "RADIO_URL:" $RADIO_URL
echo "TUN_INTERFACE_NAME:" $TUN_INTERFACE_NAME
echo "BACKBONE_INTERFACE: $BACKBONE_INTERFACE_ARG"
echo "NAT64_PREFIX:" $NAT64_PREFIX
echo "AUTO_PREFIX_ROUTE:" $AUTO_PREFIX_ROUTE
echo "AUTO_PREFIX_SLAAC:" $AUTO_PREFIX_SLAAC

NAT64_PREFIX=${NAT64_PREFIX/\//\\\/}

sed -i "s/^prefix.*$/prefix $NAT64_PREFIX/" /etc/tayga.conf
sed -i "s/dns64.*$/dns64 $NAT64_PREFIX {};/" /etc/bind/named.conf.options

echo "OTBR_AGENT_OPTS=\"-I $TUN_INTERFACE_NAME $BACKBONE_INTERFACE_ARG -d7 $RADIO_URL\"" >/etc/default/otbr-agent
echo "OTBR_WEB_OPTS=\"-I $TUN_INTERFACE_NAME -d7 -p 80\"" >/etc/default/otbr-web

/app/script/server
sleep 20

ot-ctl ifconfig down
ot-ctl thread stop
ot-ctl dataset init new
ot-ctl dataset panid 0xface
ot-ctl dataset extpanid 1111111122222222
ot-ctl dataset networkname OpenThread
ot-ctl dataset masterkey 00112233445566778899aabbccddeeff
ot-ctl dataset channel 25
ot-ctl dataset commit active
ot-ctl prefix add fd11:22::/64 pasor
ot-ctl ifconfig up
ot-ctl thread start
sleep 2
ot-ctl netdata register
ot-ctl state leader
sleep 10

ot-ctl state

tail -f /var/log/syslog
