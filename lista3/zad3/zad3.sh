#!/bin/bash
joke=$(curl -s http://api.icndb.com/jokes/random | jq .value.joke -r | sed 's/&quot;/\"/g')
img=$(curl -s http://thecatapi.com/api/images/get?format=xml\&size=small | xmllint --xpath '/response/data/images/image/url/text()' -)
curl -s $img > cat.jpg
img2txt -W 150 -f utf8 cat.jpg
echo $joke
# cat.jpg
