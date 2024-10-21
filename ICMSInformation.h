#ifndef _ICMS_INFORMATION_
#define _ICMS_INFORMATION_

#include "crow.h"

class ICMSInformation {
private:
  double icms;
  double icms_outra_uf;
  double fcp;
  double fcp_uf_dest;

public:
  crow::json::wvalue parse_to_json();
  ICMSInformation(double icms_value, double icms_outra_uf_value,
                  double fcp_value, double fcp_uf_dest_value);
  ~ICMSInformation();
};

#endif