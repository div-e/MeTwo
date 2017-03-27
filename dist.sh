#!/bin/bash

rm -r /wwwroot
cp -r ./wwwroot /
cp ./nginx.conf /etc/nginx/
nginx -s reload
