#include <exception>
#include "pqrs/xmlcompiler.hpp"

namespace pqrs {
  void
  xmlcompiler::reload_replacementdef_(void)
  {
    replacement_.clear();

    std::vector<xml_file_path_ptr> xml_file_path_ptrs;
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::private_xml, "private.xml")));
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::system_xml,  "replacementdef.xml")));

    std::vector<ptree_ptr> pt_ptrs;
    read_xmls_(pt_ptrs, xml_file_path_ptrs);

    for (auto pt_ptr : pt_ptrs) {
      traverse_replacementdef_(*pt_ptr);
    }
  }

  void
  xmlcompiler::traverse_replacementdef_(const boost::property_tree::ptree& pt)
  {
    for (auto& it : pt) {
      if (it.first != "replacementdef") {
        traverse_replacementdef_(it.second);
      } else {
        std::string name;
        std::string value;
        for (auto& child : it.second) {
          if (child.first == "replacementname") {
            name = child.second.data();
          } else if (child.first == "replacementvalue") {
            value = child.second.data();
          }
        }
        if (name.empty() || value.empty()) {
          continue;
        }
        // Adding to replacement_ if name is not found.
        if (replacement_.find(name) == replacement_.end()) {
          replacement_[name] = value;
        }
      }
    }
  }
}
