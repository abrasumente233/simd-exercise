#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "naive_base64.h"
#include "neon_base64.h"

struct Test_Case {
    const char *input;
    const char *expected;
};

static Test_Case test_cases[] = {
    {"tornado", "dG9ybmFkbw=="},
    {"Lorem ipsum do>>> \?\?\? amet, consectetur adipisci",
     "TG9yZW0gaXBzdW0gZG8+Pj4gPz8/IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2Np"},
    {"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec semper ullamcorper purus, cursus feugiat odio. Nullam risus tortor, malesuada id aliquam non, posuere in tellus. Sed consectetur turpis in sem vulputate luctus. Etiam molestie, urna quis venenatis molestie, libero lectus pellentesque ipsum, congue porttitor leo libero sed nisi. Morbi justo turpis, tristique vel blandit sed, sollicitudin et erat. Maecenas sollicitudin enim turpis. Aenean metus erat, varius quis quam quis, finibus interdum arcu. Fusce convallis sagittis turpis efficitur ullamcorper. Nunc sem justo, fermentum viverra magna nec, blandit sagittis ex. Donec tincidunt arcu id diam aliquam, ac molestie dui tincidunt.\
\
Suspendisse tempus, justo eget pretium sollicitudin, metus sem eleifend sem, eget ullamcorper justo metus et lorem. Nulla auctor arcu dolor, et posuere sapien varius at. Proin ut convallis elit. Phasellus quis malesuada elit. Ut non euismod metus. Morbi mollis ante nec arcu dignissim, ut feugiat ligula dapibus. Mauris pharetra convallis orci, nec scelerisque mauris pretium id. Integer convallis fringilla purus, in mattis neque porta id. Aliquam ornare a tortor ac dictum. Etiam vitae feugiat arcu. Sed sed turpis eget nisl placerat porttitor. Duis suscipit metus et risus condimentum ullamcorper. Nulla ullamcorper lacus arcu, quis blandit purus cursus eu. Proin mollis lacinia dolor nec faucibus. Sed placerat odio est, in dictum odio egestas sed.\
\
In tristique dolor metus, volutpat porttitor libero lacinia at. Praesent fermentum vel arcu ut gravida. Sed aliquam feugiat faucibus. Donec sed auctor magna. Fusce tortor mauris, porttitor sit amet massa ac, commodo venenatis dolor. Sed pellentesque dolor commodo, elementum tellus nec, imperdiet turpis. Quisque condimentum vel quam vitae ultricies. Pellentesque ac tellus sit amet metus commodo consequat. Vestibulum rhoncus nisi sed mauris hendrerit scelerisque ut nec magna. Vivamus hendrerit porttitor efficitur. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Donec rutrum, turpis vel luctus hendrerit, nisl arcu bibendum neque, et placerat libero felis a justo. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Fusce pharetra ligula ac ipsum molestie volutpat. Integer ultrices bibendum ex. Donec dignissim hendrerit magna, at auctor leo eleifend sed.\
\
Nam id vestibulum libero. Mauris a dictum urna, quis vestibulum tellus. Pellentesque eget dui sapien. Maecenas sodales dictum elit, et tincidunt justo congue sit amet. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Donec ac ante lorem. Praesent maximus elit at metus lacinia semper. Sed diam dui, sollicitudin ac vestibulum eget, viverra iaculis est. Maecenas imperdiet arcu eget maximus vestibulum. Duis quis dolor lacinia, feugiat nunc ac, accumsan leo. Donec bibendum, nunc ut tristique dignissim, nunc nunc dignissim arcu, vel ultricies diam ipsum a metus. Ut bibendum orci quis quam ultricies faucibus. Pellentesque pulvinar id mi vitae tristique. Nulla facilisi. Donec felis risus, lacinia nec varius nec, pharetra eget lorem. Ut eu nibh sit amet nibh dignissim pulvinar eget vehicula turpis.\
\
Vivamus pretium hendrerit blandit. Sed sit amet nisi mauris. Curabitur nec turpis sit amet ex elementum suscipit tincidunt eget arcu. Duis dolor eros, lacinia sit amet sollicitudin tincidunt, finibus id risus. Quisque porttitor, sapien in placerat sodales, erat ligula dapibus ligula, quis vehicula urna dui non tortor. Vivamus eu fermentum neque, ac sagittis massa. Sed varius magna sed auctor tincidunt. Quisque est ligula, suscipit et velit ac, sagittis mattis diam.",
     "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2NpbmcgZW"
     "xpdC4gRG9uZWMgc2VtcGVyIHVsbGFtY29ycGVyIHB1cnVzLCBjdXJzdXMgZmV1Z2lhdCBv"
     "ZGlvLiBOdWxsYW0gcmlzdXMgdG9ydG9yLCBtYWxlc3VhZGEgaWQgYWxpcXVhbSBub24sIH"
     "Bvc3VlcmUgaW4gdGVsbHVzLiBTZWQgY29uc2VjdGV0dXIgdHVycGlzIGluIHNlbSB2dWxw"
     "dXRhdGUgbHVjdHVzLiBFdGlhbSBtb2xlc3RpZSwgdXJuYSBxdWlzIHZlbmVuYXRpcyBtb2"
     "xlc3RpZSwgbGliZXJvIGxlY3R1cyBwZWxsZW50ZXNxdWUgaXBzdW0sIGNvbmd1ZSBwb3J0"
     "dGl0b3IgbGVvIGxpYmVybyBzZWQgbmlzaS4gTW9yYmkganVzdG8gdHVycGlzLCB0cmlzdG"
     "lxdWUgdmVsIGJsYW5kaXQgc2VkLCBzb2xsaWNpdHVkaW4gZXQgZXJhdC4gTWFlY2VuYXMg"
     "c29sbGljaXR1ZGluIGVuaW0gdHVycGlzLiBBZW5lYW4gbWV0dXMgZXJhdCwgdmFyaXVzIH"
     "F1aXMgcXVhbSBxdWlzLCBmaW5pYnVzIGludGVyZHVtIGFyY3UuIEZ1c2NlIGNvbnZhbGxp"
     "cyBzYWdpdHRpcyB0dXJwaXMgZWZmaWNpdHVyIHVsbGFtY29ycGVyLiBOdW5jIHNlbSBqdX"
     "N0bywgZmVybWVudHVtIHZpdmVycmEgbWFnbmEgbmVjLCBibGFuZGl0IHNhZ2l0dGlzIGV4"
     "LiBEb25lYyB0aW5jaWR1bnQgYXJjdSBpZCBkaWFtIGFsaXF1YW0sIGFjIG1vbGVzdGllIG"
     "R1aSB0aW5jaWR1bnQuU3VzcGVuZGlzc2UgdGVtcHVzLCBqdXN0byBlZ2V0IHByZXRpdW0g"
     "c29sbGljaXR1ZGluLCBtZXR1cyBzZW0gZWxlaWZlbmQgc2VtLCBlZ2V0IHVsbGFtY29ycG"
     "VyIGp1c3RvIG1ldHVzIGV0IGxvcmVtLiBOdWxsYSBhdWN0b3IgYXJjdSBkb2xvciwgZXQg"
     "cG9zdWVyZSBzYXBpZW4gdmFyaXVzIGF0LiBQcm9pbiB1dCBjb252YWxsaXMgZWxpdC4gUG"
     "hhc2VsbHVzIHF1aXMgbWFsZXN1YWRhIGVsaXQuIFV0IG5vbiBldWlzbW9kIG1ldHVzLiBN"
     "b3JiaSBtb2xsaXMgYW50ZSBuZWMgYXJjdSBkaWduaXNzaW0sIHV0IGZldWdpYXQgbGlndW"
     "xhIGRhcGlidXMuIE1hdXJpcyBwaGFyZXRyYSBjb252YWxsaXMgb3JjaSwgbmVjIHNjZWxl"
     "cmlzcXVlIG1hdXJpcyBwcmV0aXVtIGlkLiBJbnRlZ2VyIGNvbnZhbGxpcyBmcmluZ2lsbG"
     "EgcHVydXMsIGluIG1hdHRpcyBuZXF1ZSBwb3J0YSBpZC4gQWxpcXVhbSBvcm5hcmUgYSB0"
     "b3J0b3IgYWMgZGljdHVtLiBFdGlhbSB2aXRhZSBmZXVnaWF0IGFyY3UuIFNlZCBzZWQgdH"
     "VycGlzIGVnZXQgbmlzbCBwbGFjZXJhdCBwb3J0dGl0b3IuIER1aXMgc3VzY2lwaXQgbWV0"
     "dXMgZXQgcmlzdXMgY29uZGltZW50dW0gdWxsYW1jb3JwZXIuIE51bGxhIHVsbGFtY29ycG"
     "VyIGxhY3VzIGFyY3UsIHF1aXMgYmxhbmRpdCBwdXJ1cyBjdXJzdXMgZXUuIFByb2luIG1v"
     "bGxpcyBsYWNpbmlhIGRvbG9yIG5lYyBmYXVjaWJ1cy4gU2VkIHBsYWNlcmF0IG9kaW8gZX"
     "N0LCBpbiBkaWN0dW0gb2RpbyBlZ2VzdGFzIHNlZC5JbiB0cmlzdGlxdWUgZG9sb3IgbWV0"
     "dXMsIHZvbHV0cGF0IHBvcnR0aXRvciBsaWJlcm8gbGFjaW5pYSBhdC4gUHJhZXNlbnQgZm"
     "VybWVudHVtIHZlbCBhcmN1IHV0IGdyYXZpZGEuIFNlZCBhbGlxdWFtIGZldWdpYXQgZmF1"
     "Y2lidXMuIERvbmVjIHNlZCBhdWN0b3IgbWFnbmEuIEZ1c2NlIHRvcnRvciBtYXVyaXMsIH"
     "BvcnR0aXRvciBzaXQgYW1ldCBtYXNzYSBhYywgY29tbW9kbyB2ZW5lbmF0aXMgZG9sb3Iu"
     "IFNlZCBwZWxsZW50ZXNxdWUgZG9sb3IgY29tbW9kbywgZWxlbWVudHVtIHRlbGx1cyBuZW"
     "MsIGltcGVyZGlldCB0dXJwaXMuIFF1aXNxdWUgY29uZGltZW50dW0gdmVsIHF1YW0gdml0"
     "YWUgdWx0cmljaWVzLiBQZWxsZW50ZXNxdWUgYWMgdGVsbHVzIHNpdCBhbWV0IG1ldHVzIG"
     "NvbW1vZG8gY29uc2VxdWF0LiBWZXN0aWJ1bHVtIHJob25jdXMgbmlzaSBzZWQgbWF1cmlz"
     "IGhlbmRyZXJpdCBzY2VsZXJpc3F1ZSB1dCBuZWMgbWFnbmEuIFZpdmFtdXMgaGVuZHJlcm"
     "l0IHBvcnR0aXRvciBlZmZpY2l0dXIuIFBlbGxlbnRlc3F1ZSBoYWJpdGFudCBtb3JiaSB0"
     "cmlzdGlxdWUgc2VuZWN0dXMgZXQgbmV0dXMgZXQgbWFsZXN1YWRhIGZhbWVzIGFjIHR1cn"
     "BpcyBlZ2VzdGFzLiBEb25lYyBydXRydW0sIHR1cnBpcyB2ZWwgbHVjdHVzIGhlbmRyZXJp"
     "dCwgbmlzbCBhcmN1IGJpYmVuZHVtIG5lcXVlLCBldCBwbGFjZXJhdCBsaWJlcm8gZmVsaX"
     "MgYSBqdXN0by4gQ2xhc3MgYXB0ZW50IHRhY2l0aSBzb2Npb3NxdSBhZCBsaXRvcmEgdG9y"
     "cXVlbnQgcGVyIGNvbnViaWEgbm9zdHJhLCBwZXIgaW5jZXB0b3MgaGltZW5hZW9zLiBGdX"
     "NjZSBwaGFyZXRyYSBsaWd1bGEgYWMgaXBzdW0gbW9sZXN0aWUgdm9sdXRwYXQuIEludGVn"
     "ZXIgdWx0cmljZXMgYmliZW5kdW0gZXguIERvbmVjIGRpZ25pc3NpbSBoZW5kcmVyaXQgbW"
     "FnbmEsIGF0IGF1Y3RvciBsZW8gZWxlaWZlbmQgc2VkLk5hbSBpZCB2ZXN0aWJ1bHVtIGxp"
     "YmVyby4gTWF1cmlzIGEgZGljdHVtIHVybmEsIHF1aXMgdmVzdGlidWx1bSB0ZWxsdXMuIF"
     "BlbGxlbnRlc3F1ZSBlZ2V0IGR1aSBzYXBpZW4uIE1hZWNlbmFzIHNvZGFsZXMgZGljdHVt"
     "IGVsaXQsIGV0IHRpbmNpZHVudCBqdXN0byBjb25ndWUgc2l0IGFtZXQuIFZlc3RpYnVsdW"
     "0gYW50ZSBpcHN1bSBwcmltaXMgaW4gZmF1Y2lidXMgb3JjaSBsdWN0dXMgZXQgdWx0cmlj"
     "ZXMgcG9zdWVyZSBjdWJpbGlhIGN1cmFlOyBEb25lYyBhYyBhbnRlIGxvcmVtLiBQcmFlc2"
     "VudCBtYXhpbXVzIGVsaXQgYXQgbWV0dXMgbGFjaW5pYSBzZW1wZXIuIFNlZCBkaWFtIGR1"
     "aSwgc29sbGljaXR1ZGluIGFjIHZlc3RpYnVsdW0gZWdldCwgdml2ZXJyYSBpYWN1bGlzIG"
     "VzdC4gTWFlY2VuYXMgaW1wZXJkaWV0IGFyY3UgZWdldCBtYXhpbXVzIHZlc3RpYnVsdW0u"
     "IER1aXMgcXVpcyBkb2xvciBsYWNpbmlhLCBmZXVnaWF0IG51bmMgYWMsIGFjY3Vtc2FuIG"
     "xlby4gRG9uZWMgYmliZW5kdW0sIG51bmMgdXQgdHJpc3RpcXVlIGRpZ25pc3NpbSwgbnVu"
     "YyBudW5jIGRpZ25pc3NpbSBhcmN1LCB2ZWwgdWx0cmljaWVzIGRpYW0gaXBzdW0gYSBtZX"
     "R1cy4gVXQgYmliZW5kdW0gb3JjaSBxdWlzIHF1YW0gdWx0cmljaWVzIGZhdWNpYnVzLiBQ"
     "ZWxsZW50ZXNxdWUgcHVsdmluYXIgaWQgbWkgdml0YWUgdHJpc3RpcXVlLiBOdWxsYSBmYW"
     "NpbGlzaS4gRG9uZWMgZmVsaXMgcmlzdXMsIGxhY2luaWEgbmVjIHZhcml1cyBuZWMsIHBo"
     "YXJldHJhIGVnZXQgbG9yZW0uIFV0IGV1IG5pYmggc2l0IGFtZXQgbmliaCBkaWduaXNzaW"
     "0gcHVsdmluYXIgZWdldCB2ZWhpY3VsYSB0dXJwaXMuVml2YW11cyBwcmV0aXVtIGhlbmRy"
     "ZXJpdCBibGFuZGl0LiBTZWQgc2l0IGFtZXQgbmlzaSBtYXVyaXMuIEN1cmFiaXR1ciBuZW"
     "MgdHVycGlzIHNpdCBhbWV0IGV4IGVsZW1lbnR1bSBzdXNjaXBpdCB0aW5jaWR1bnQgZWdl"
     "dCBhcmN1LiBEdWlzIGRvbG9yIGVyb3MsIGxhY2luaWEgc2l0IGFtZXQgc29sbGljaXR1ZG"
     "luIHRpbmNpZHVudCwgZmluaWJ1cyBpZCByaXN1cy4gUXVpc3F1ZSBwb3J0dGl0b3IsIHNh"
     "cGllbiBpbiBwbGFjZXJhdCBzb2RhbGVzLCBlcmF0IGxpZ3VsYSBkYXBpYnVzIGxpZ3VsYS"
     "wgcXVpcyB2ZWhpY3VsYSB1cm5hIGR1aSBub24gdG9ydG9yLiBWaXZhbXVzIGV1IGZlcm1l"
     "bnR1bSBuZXF1ZSwgYWMgc2FnaXR0aXMgbWFzc2EuIFNlZCB2YXJpdXMgbWFnbmEgc2VkIG"
     "F1Y3RvciB0aW5jaWR1bnQuIFF1aXNxdWUgZXN0IGxpZ3VsYSwgc3VzY2lwaXQgZXQgdmVs"
     "aXQgYWMsIHNhZ2l0dGlzIG1hdHRpcyBkaWFtLg=="}};

static inline size_t encoded_length(size_t bytes) {
    size_t round_up = (bytes + 2) / 3 * 3;
    return round_up * 4;
}

template <typename F>
void check_encode(const char *name, F encode) {
    printf("testing %s ", name);
    bool failed = false;
    for (auto &test_case : test_cases) {
        uint8_t *buf =
            (uint8_t *)malloc(encoded_length(strlen(test_case.input)) + 1);
        size_t bytes = strlen(test_case.input);
        encode((const uint8_t *)test_case.input, bytes, buf);
        if (memcmp(buf, test_case.expected, bytes) != 0) {
            printf("x");
            failed = true;
        } else {
            printf(".");
        }
    }
    if (failed) {
        printf(" \e[91m[failed]\033[0m\n");
    } else {
        printf(" \e[32m[passed]\033[0m\n");
    }
}

int main() {
    check_encode("scalar_impl", naive_base64::encode);
    check_encode("neon_impl", neon_base64::encode);
}
