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
  unordered_map_body.push(`    {"${keys[i]}", "${characters[keys[i]].characters.replace(/\\/g, '\\\\')}"}`);
}

const filename_arg = process.argv[process.argv.length - 1];
const filename = path.basename(filename_arg, path.extname(filename_arg));

const src_h = `#pragma once
#include <unordered_map>
#include <string>

namespace yourhtml {

std::unordered_map<std::string, std::string> &get_character_reference_lookup_map();

std::string lookup_characters(const std::string &key);

}
`;

const src_cc = `#include <yourhtml/${filename}.h>

namespace yourhtml {

std::unordered_map<std::string, std::string> &get_character_reference_lookup_map() {
  static std::unordered_map<std::string, std::string> character_map({
    ${unordered_map_body.join(',\n').trim()}
  });
  return character_map;
}

std::string lookup_characters(const std::string &key) {
  const auto &character_map = get_character_reference_lookup_map();
  auto found = character_map.find(key);
  if (found == character_map.end()) {
    return "";
  }
  return found->second;
}

} // yourhtml
`;

fs.writeFileSync(path.resolve(__dirname, '..', 'yourhtml', `${filename}.h`), src_h)
fs.writeFileSync(path.resolve(__dirname, '..', 'yourhtml', `${filename}.cc`), src_cc);
