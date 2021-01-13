set -e

CHANNELS=1
OVERSAMPLE=1
INPUT=plugins.json
OUTPUT=output
NAME=foo
VERSION=0

while getopts c:x:o:n:v:i:h option; do
  case "${option}" in
    c) CHANNELS=${OPTARG};;
    x) OVERSAMPLE=${OPTARG};;
    n) NAME=${OPTARG};;
    v) VERSION=${OPTARG};;
    i) INPUT=${OPTARG};;
    o) OUTPUT=${OPTARG};;
    h) echo "Use the source, Luke!"; exit
  esac
done

echo Arguments:
echo

echo channels: ${CHANNELS} 
echo oversample: ${OVERSAMPLE} 
echo name: ${NAME} 
echo version: ${VERSION} 
echo input: ${INPUT}
echo output: ${OUTPUT}

echo

echo Generating plugin specific ttl: ${OUTPUT}.ttl

uri=$(cat ${INPUT} | jq .\"uri\" | sed -e 's/${name}/'"${NAME}/g")
echo ${uri}


echo Generating bundle manifest snippet: ${OUTPUT}_manifest.ttl

echo Generating C++ source ${output}.cc
