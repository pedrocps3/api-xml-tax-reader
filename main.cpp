#include "ICMSInformation.h"
#include "crow.h"
#include "tinyxml2.h"
#include <cstdlib>
#include <future>
#include <stdexcept>

tinyxml2::XMLElement *get_xml_node(tinyxml2::XMLElement *const root_element,
                                   const char *tag_name) {
  tinyxml2::XMLElement *tag = root_element->FirstChildElement(tag_name);
  if (tag == nullptr)
    throw std::runtime_error(std::string("error to get value from tag ") +
                             tag_name);

  return tag;
}

double parse_xml_value(const char *value, const char *tag_name) {
  char *endptr;
  double converted_value = std::strtod(value, &endptr);

  if (*endptr != '\0') {
    std::cout << "error to parse " << tag_name << std::endl;

    converted_value = 0;
  }

  return converted_value;
}

double get_v_icms(tinyxml2::XMLElement *const document_root) {
  tinyxml2::XMLElement *tag_total = get_xml_node(document_root, "total");
  tinyxml2::XMLElement *tag_icms_tot = get_xml_node(tag_total, "ICMSTot");
  tinyxml2::XMLElement *tag_v_icms = get_xml_node(tag_icms_tot, "vICMS");

  const char *v_icms = tag_v_icms->GetText();

  return parse_xml_value(v_icms, "vICMS");
}

double get_v_icms_uf_dest(tinyxml2::XMLElement *const document_root) {
  tinyxml2::XMLElement *tag_det = get_xml_node(document_root, "det");
  tinyxml2::XMLElement *tag_imposto = get_xml_node(tag_det, "imposto");
  tinyxml2::XMLElement *tag_icms_uf_dest =
      get_xml_node(tag_imposto, "ICMSUFDest");
  tinyxml2::XMLElement *tag_v_icms_uf_dest =
      get_xml_node(tag_icms_uf_dest, "vICMSUFDest");

  const char *v_icms_uf_dest = tag_v_icms_uf_dest->GetText();

  return parse_xml_value(v_icms_uf_dest, "vICMSUFDest");
}

double get_v_fcp(tinyxml2::XMLElement *const document_root) {
  tinyxml2::XMLElement *tag_total = get_xml_node(document_root, "total");
  tinyxml2::XMLElement *tag_icms_tot = get_xml_node(tag_total, "ICMSTot");
  tinyxml2::XMLElement *tag_v_fcp = get_xml_node(tag_icms_tot, "vFCP");

  const char *v_fcp = tag_v_fcp->GetText();

  return parse_xml_value(v_fcp, "vFCP");
}

double get_v_fcp_uf_dest(tinyxml2::XMLElement *const document_root) {
  tinyxml2::XMLElement *tag_det = get_xml_node(document_root, "det");
  tinyxml2::XMLElement *tag_imposto = get_xml_node(tag_det, "imposto");
  tinyxml2::XMLElement *tag_icms_uf_dest =
      get_xml_node(tag_imposto, "ICMSUFDest");
  tinyxml2::XMLElement *tag_v_fcp_uf_dest =
      get_xml_node(tag_icms_uf_dest, "vFCPUFDest");

  const char *v_fcp_uf_dest = tag_v_fcp_uf_dest->GetText();
  return parse_xml_value(v_fcp_uf_dest, "vFCPUFDest");
}

crow::json::wvalue get_icms_informations(tinyxml2::XMLDocument &xml_document) {
  tinyxml2::XMLElement *root = xml_document.RootElement();
  if (root == nullptr)
    throw std::runtime_error("error to get xml root value");

  tinyxml2::XMLElement *document_root = root->FirstChildElement("NFe");
  if (document_root == nullptr)
    throw std::runtime_error("error to get xml NFe value");

  tinyxml2::XMLElement *tag_infnFe = document_root->FirstChildElement("infNFe");
  if (tag_infnFe == nullptr)
    throw std::runtime_error("error to get xml infNFe value");

  double v_icms = get_v_icms(tag_infnFe);
  double v_icms_uf_dest = get_v_icms_uf_dest(tag_infnFe);
  double v_fcp = get_v_fcp(tag_infnFe);
  double v_fcp_uf_dest = get_v_fcp_uf_dest(tag_infnFe);

  ICMSInformation icms_info{v_icms, v_icms_uf_dest, v_fcp, v_fcp_uf_dest};

  return icms_info.parse_to_json();
}

int main() {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/monitoramento")
  ([]() { return crow::response(200, "hello world"); });

  CROW_ROUTE(app, "/ler-xml")
      .methods("POST"_method)([](const crow::request &req) {
        crow::json::rvalue json_body = crow::json::load(req.body);
        if (!json_body)
          return crow::response(400, "invalid json body");

        std::string xml_value = json_body["xml_value"].s();
        if (xml_value.empty())
          return crow::response(400, "invalid xml value");

        tinyxml2::XMLDocument xml_document;
        tinyxml2::XMLError result = xml_document.Parse(xml_value.c_str());

        if (result != tinyxml2::XML_SUCCESS)
          return crow::response(400, "error to parse xml");

        std::future handle = std::async(
            std::launch::async, get_icms_informations, std::ref(xml_document));

        crow::json::wvalue response = handle.get();

        return crow::response(200, response);
      });

  app.port(5051).multithreaded().run();
}
