#include "ICMSInformation.h"

#include "crow.h"

ICMSInformation::ICMSInformation(double icms_value, double icms_outra_uf_value,
                                 double fcp_value, double fcp_uf_dest_value)
    : icms{icms_value}, icms_outra_uf{icms_outra_uf_value}, fcp{fcp_value},
      fcp_uf_dest{fcp_uf_dest_value} {}

ICMSInformation::~ICMSInformation(){};

crow::json::wvalue ICMSInformation::parse_to_json() {
  crow::json::wvalue json_value;

  json_value["icms"] = icms;
  json_value["icms_outra_uf"] = icms_outra_uf;
  json_value["fcp"] = fcp;
  json_value["fcp_uf_dest"] = fcp_uf_dest;

  return json_value;
}