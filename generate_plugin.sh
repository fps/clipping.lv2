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

uri=$(cat ${INPUT} | jq -r  .\"uri\" | sed -e 's/${name}/'"${NAME}/g" | sed -e 's/${oversample}/'"${OVERSAMPLE}/g" | sed -e 's/${channels}/'"${CHANNELS}/g" | sed -e 's/${version}/'"${VERSION}/g")

echo URI: ${uri}

echo '@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .' > ${OUTPUT}.ttl
echo '@prefix foaf: <http://xmlns.com/foaf/0.1/> .' >> ${OUTPUT}.ttl
echo '@prefix doap: <http://usefulinc.com/ns/doap#> .' >> ${OUTPUT}.ttl
echo " " >> ${OUTPUT}.ttl
echo "<${uri}> a lv2:Plugin ;" >> ${OUTPUT}.ttl



echo Generating bundle manifest snippet: ${OUTPUT}_manifest.ttl

echo Generating C++ source ${output}.cc
