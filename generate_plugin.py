from optparse import OptionParser
import json

option_parser = OptionParser()
option_parser.add_option("-i", "--input", dest="input", help="A JSON file containing a plugin description", default="plugins.json")
option_parser.add_option("-x", "--oversampling-factor", dest="oversample", help="The oversampling factor used", default="1")
option_parser.add_option("-c", "--channels", dest="channels", help="The audio input/output channels count", default="1")
option_parser.add_option("-n", "--name", dest="name", help="The plugin name", default="name")
option_parser.add_option("-v", "--version", dest="version", help="The plugin version", default="1")
option_parser.add_option("-o", "--output", dest="output", help="The output file base name", default="output")
option_parser.add_option("-d", "--description", dest="description", help="The description", default="A plugin")

(options, args) = option_parser.parse_args()

def replace_options_in_string(s, options):
  return s.replace("${name}", options.name).replace("${version}", options.version).replace("${channels}", str(options.channels)).replace("${oversample}", str(options.oversample))

with open(options.input) as input_file:
  input_json = json.load(input_file)
  with open(options.output + ".ttl", "w") as ttl_output_file, open(options.output + ".manifest.ttl", "w") as manifest_output_file:
    ttl_output_file.write("@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .\n")
    ttl_output_file.write("@prefix foaf: <http://xmlns.com/foaf/0.1/> .\n")
    ttl_output_file.write("@prefix doap: <http://usefulinc.com/ns/doap#> .\n")
    ttl_output_file.write("\n")
    uri = replace_options_in_string(input_json["uri"], options)
    ttl_output_file.write(f"<{uri}> a lv2:Plugin ;\n")
    name = replace_options_in_string(input_json["name"], options)
    ttl_output_file.write(f"  doap:name \"{name}\" ;\n")
    ttl_output_file.write("  doap:license <http://usefulinc.com/doap/licenses/gpl> ;\n")
    ttl_output_file.write(f"  doap:description \"{options.description}\" ;\n")
    ttl_output_file.write(f"  lv2:binary <{name}.so>\n")
    ttl_output_file.write("  lv2:optionalFeature lv2:hardRTCapable ;\n")


    port_index = 0
    first = True
    ttl_output_file.write("  lv2:port\n")
    for multi_port in input_json["multi-channel-ports"]:
      for channel_index in range(0, int(options.channels)):
        if not first:
          ttl_output_file.write("  ,\n")
        first = False
        ttl_output_file.write("  [\n")
        for a in multi_port["a"]:
          ttl_output_file.write(f"    a {a} ;\n")
        ttl_output_file.write(f"    lv2:index {port_index} ;\n")
        port_index += 1
        name = multi_port["name"].replace("${channel}", f"{channel_index}")
        ttl_output_file.write(f"    lv2:name \"{name}\" ;\n")
        symbol = multi_port["symbol"].replace("${channel}", f"{channel_index}")
        ttl_output_file.write(f"    lv2:symbol \"{symbol}\" ;\n")
        ttl_output_file.write("  ]\n")

    for port in input_json["ports"]:
      if not first:
        ttl_output_file.write("  ,\n")
      first = False
      ttl_output_file.write("  [\n")
      for a in port["a"]:
        ttl_output_file.write(f"    a {a} ;\n")
      ttl_output_file.write(f"    lv2:index {port_index} ;\n")
      port_index += 1
      name = port["name"]
      ttl_output_file.write(f"    lv2:name \"{name}\" ;\n")
      symbol = port["symbol"]
      ttl_output_file.write(f"    lv2:symbol \"{symbol}\" ;\n")
      ttl_output_file.write("  ]\n")

    ttl_output_file.write("  .\n")
      
       
