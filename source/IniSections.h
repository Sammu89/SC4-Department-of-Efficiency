// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
// ONE configuration file, DepartmentEfficiency.ini, with three independent sections: [AutoFinance] (health, education), [UtilityResearch] (power, water, sanitation, regional
// contracts) and [LotDemolition] (automatic demolition). Every module keeps its own parser and its own defaults: this file only cuts the text into sections and writes a key
// inside ONE section (a repeated key such as Mode belongs to the section it is in, so a change in one section can never touch another). Pure text, no game types.
#pragma once

#include <string>
#include <utility>
#include <vector>

namespace sc4af {

constexpr const char* kIniFileName = "DepartmentEfficiency.ini";
constexpr const char* kIniSecAutoFinance = "AutoFinance";
constexpr const char* kIniSecUtility = "UtilityResearch";
constexpr const char* kIniSecDemolition = "LotDemolition";

namespace inisec {

inline std::string Trim(const std::string& s) {
    const auto b = s.find_first_not_of(" \t\r");
    if (b == std::string::npos) return std::string();
    return s.substr(b, s.find_last_not_of(" \t\r") - b + 1);
}
inline std::string Lower(std::string s) {
    for (char& c : s) if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    return s;
}
// the name of a section header line ("[ Name ]" -> "name", lower case), "" when the line is not a header
inline std::string HeaderName(const std::string& line) {
    const std::string t = Trim(line);
    if (t.size() < 2 || t.front() != '[' || t.back() != ']') return std::string();
    return Lower(Trim(t.substr(1, t.size() - 2)));
}
// [begin, end) of the FIRST section called `section` (from its header line to the next header line or the end of the text); begin == npos when the section is absent
inline std::pair<std::size_t, std::size_t> SectionRange(const std::string& text, const std::string& section) {
    const std::string want = Lower(section);
    std::size_t begin = std::string::npos;
    for (std::size_t pos = 0; pos < text.size();) {
        auto eol = text.find('\n', pos);
        const std::size_t next = eol == std::string::npos ? text.size() : eol + 1;
        const std::string name = HeaderName(text.substr(pos, (eol == std::string::npos ? text.size() : eol) - pos));
        if (!name.empty()) {
            if (begin != std::string::npos) return {begin, pos};
            if (name == want) begin = pos;
        }
        pos = next;
    }
    return {begin, text.size()};
}

}  // namespace inisec

// The exact text of one section (its header line included), "" when the file has no such section: what a module's own parser is given.
inline std::string IniSectionText(const std::string& file, const std::string& section) {
    const auto r = inisec::SectionRange(file, section);
    return r.first == std::string::npos ? std::string() : file.substr(r.first, r.second - r.first);
}

// `file` with the text of `section` replaced by `sectionText` (an absent section is appended after a blank line); every other byte of the file is kept.
inline std::string IniReplaceSection(const std::string& file, const std::string& section, const std::string& sectionText) {
    const auto r = inisec::SectionRange(file, section);
    if (r.first != std::string::npos) return file.substr(0, r.first) + sectionText + file.substr(r.second);
    const std::string nl = file.find("\r\n") != std::string::npos ? "\r\n" : "\n";
    std::string out = file;
    if (!out.empty() && out.back() != '\n') out += nl;
    if (!out.empty()) out += nl;
    return out + sectionText;
}

}  // namespace sc4af
