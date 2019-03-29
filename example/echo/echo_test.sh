#!/bin/sh

set -e

./example/echo/server&
./example/echo/client
