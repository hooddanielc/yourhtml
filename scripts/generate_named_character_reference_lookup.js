#!/usr/bin/env node

// generate c++ lookup function for html named character references.

const fs = require('fs');
const path = require('path');
const process = require('process');

if (process.argv.indexOf('-h') > -1) {
  console.log('Usage: generate_named_character_reference_lookup <output.h>');
}

const characters_path = path.resolve(
  __dirname,
  '..',
  'assets',
  'supported_named_character_references.json'
);

const characters = JSON.parse(fs.readFileSync(characters_path, 'utf8'));
const keys = Object.keys(characters);

// generate an unordered map assignment
const unordered_map_body = [];
for (let i = 0;i < keys.length; ++i) {
  const {codepoints} = characters[keys[i]];
  unordered_map_body.push(`    {"${keys[i]}", std::vector<unsigned int>({${codepoints.join(',')}})}`);
}

const arg_len = process.argv.length;
const filename_arg = arg_len > 2 ? process.argv[process.argv.length - 1] : path.resolve(
  __dirname,
  '..',
  'lookup_character_reference'
);
const filename = path.basename(filename_arg, path.extname(filename_arg));

const src_h = `#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <sstream>

namespace yourhtml {

std::unordered_map<std::string, std::vector<unsigned int>> &get_character_reference_lookup_map();

std::vector<unsigned int> lookup_characters(const std::string &key);

std::string convert_codepoints_to_utf8(const std::vector<unsigned int> &codepoints);

}
`;

const src_cc = `#include <yourhtml/${filename}.h>

namespace yourhtml {

std::unordered_map<std::string, std::vector<unsigned int>> &get_character_reference_lookup_map() {
  static std::unordered_map<std::string, std::vector<unsigned int>> character_map({
    ${unordered_map_body.join(',\n').trim()}
  });
  return character_map;
}

std::vector<unsigned int> lookup_characters(const std::string &key) {
  const auto &character_map = get_character_reference_lookup_map();
  auto found = character_map.find(key);
  if (found == character_map.end()) {
    return std::vector<unsigned int>();
  }
  return found->second;
}

std::string convert_codepoints_to_utf8(const std::vector<unsigned int> &codepoints) {
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
  std::stringstream ss;
  for (const auto &num: codepoints) {
    ss << converter.to_bytes(num);
  }
  return ss.str();
}

} // yourhtml
`;

fs.writeFileSync(path.resolve(__dirname, '..', 'yourhtml', `${filename}.h`), src_h)
fs.writeFileSync(path.resolve(__dirname, '..', 'yourhtml', `${filename}.cc`), src_cc);
