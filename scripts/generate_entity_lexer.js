#!/usr/bin/env node

const fs = require('fs');
const path = require('path');
const process = require('process');
const child_process = require('child_process');

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
const rules = [];
const entity_names = [];

Object.keys(characters).forEach((key, index) => {
  const name = key;
  entity_names.push(`  "${key}",`);
  const u8char = characters[key].codepoints
    .map((i) => {
      if (String.fromCodePoint(i) === '\n') {
        return `'\\n'`;
      } else if (String.fromCodePoint(i) === '"') {
        return "'\"'";
      } else if (String.fromCodePoint(i) === '\\') {
        return "'\\\\'";
      }
      return `u8"${String.fromCodePoint(i)}"`;
    })
    .join(' << ');

  rules.push(`"${key}"    yyout << ${u8char}; return ${index};`);
});

const input = `
%option noyywrap

%%
.       yyout << YYText(); return -1;
${rules.join('\n')}
%%

`;

const read_flex_lexer_h = () => {
  if (fs.existsSync('/usr/include/FlexLexer.h')) {
    return fs.readFileSync('/usr/include/FlexLexer.h', 'utf8');
  } else if (fs.existsSync('/usr/local/include/FlexLexer.h')) {
    return fs.readFileSync('/usr/local/include/FlexLexer.h', 'utf8');
  } else {
    throw new Error('unable to find FlexLexer.h in /usr/include || /usr/local/include');
  }
}

fs.writeFileSync('/tmp/input.flex', input);
const output_h = path.resolve(__dirname, '..', 'yourhtml_entities/entity_lexer_flex.h');
const output = path.resolve(__dirname, '..', 'yourhtml_entities/entity_lexer_flex.cc');
const cmd = `flex -o ${output} --c++ /tmp/input.flex`;
console.log('executing: ', cmd);
child_process.exec(cmd, (err, res) => {
  if (err) {
    console.log('exec returned result:', err, res);
    throw new err;
  }
  console.log('successfully generated yourhtml_entities/entity_lexer_flex.cc');

  // rename
  const entity_lexer_src = fs.readFileSync(output, 'utf8')
    .replace('<FlexLexer.h>', "<yourhtml_entities/entity_lexer_flex.h>");
  const entity_lexer_src_h = read_flex_lexer_h();
  fs.writeFileSync(output, `
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-macros"
    #pragma clang diagnostic ignored "-Wreserved-id-macro"
    #pragma clang diagnostic ignored "-Wweak-vtables"
    #pragma clang diagnostic ignored "-Wreserved-id-macro"
    #pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
    #pragma clang diagnostic ignored "-Wimplicit-fallthrough"
    ${entity_lexer_src}
    #pragma clang diagnostic pop\n`.replace(/u8"/g, '"')
  );
  fs.writeFileSync(output_h, `
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-macros"
    #pragma clang diagnostic ignored "-Wreserved-id-macro"
    #pragma clang diagnostic ignored "-Wweak-vtables"
    #pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
    ${entity_lexer_src_h}
    #pragma clang diagnostic pop\n`.replace(/u8"/g, '"')
  );
});


const entity_name_array_h = `#pragma once
namespace yourhtml {

static const char *entity_names[] = {
${entity_names.join("\n")}
};

}
`;

const entity_names_path = path.resolve(__dirname, '..', 'yourhtml_entities', 'entity_names.h');
fs.writeFileSync(entity_names_path, entity_name_array_h);
