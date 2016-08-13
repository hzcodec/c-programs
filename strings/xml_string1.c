/*
    Auther      : Heinz Samuelsson
    Date        : 2015-09-19
    File        : xml_string1.c
    Reference   : -
    Description : Build an XML-string and print it out.
*/

#include <stdio.h>
#include <stdlib.h>


const char TEST_RESPONSE_HEAD[] = "\
<?xml version=\"1.0\"?>\n\
<methodResponse>\n\
  <params>\n\
    <param>\n\
      <value>\n\
        <struct>\n\
          <member>\n\
            <name>Result</name>\n\
            <value>\n\
              <array>\n";

const char TEST_CASE_RESULT[] = "\
                <data>\n\
                  <value>\n\
                    <struct>\n\
                      <member>\n\
                        <name>PortName</name>\n\
                        <value>\n\
                          <string>%s</string>\n\
                        </value>\n\
                      </member>\n\
                    </struct>\n\
                  </value>\n\
                </data>\n";

const char TEST_RESPONSE_TAIL[] = "\
              </array>\n\
            </value>\n\
          </member>\n\
        </struct>\n\
      </value>\n\
    </param>\n\
  </params>\n\
</methodResponse>\n";


int main()
{
    char stringA[10000];

    printf("=============================================================\n");

    // build xml string
    sprintf(stringA, TEST_RESPONSE_HEAD);
    printf("%s", stringA);

    sprintf(stringA, TEST_CASE_RESULT, "Port A");
    printf("%s", stringA);

    sprintf(stringA, TEST_RESPONSE_TAIL);
    printf("%s\n", stringA);

    printf("=============================================================\n");

    return 0;
}

