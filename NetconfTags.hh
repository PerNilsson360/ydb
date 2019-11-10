#ifndef _NETCONF_TAGS_HH_
#define _NETCONF_TAGS_HH_

#include "YdbStr.hh"

class NetconfIdentities
{
public:
    static const YdbStr RPC;
    static const YdbStr RPC_REPLY;
/*
The <rpc-error> element is sent in <rpc-reply> messages if an error
occurs during the processing of an <rpc> request.

If a server encounters multiple errors during the processing of an
<rpc> request, the <rpc-reply> MAY contain multiple <rpc-error>
elements.  However, a server is not required to detect or report more
than one <rpc-error> element, if a request contains multiple errors.
A server is not required to check for particular error conditions in
a specific sequence.  A server MUST return an <rpc-error> element if
any error conditions occur during processing.

A server MUST NOT return application-level- or data-model-specific
error information in an <rpc-error> element for which the client does
not have sufficient access rights.

The <rpc-error> element includes the following information:
*/
    static const YdbStr RPC_ERROR;
    
/*
error-type:  Defines the conceptual layer that the error occurred.
Enumeration.  One of:

*  transport (layer: Secure Transport)

*  rpc (layer: Messages)

*  protocol (layer: Operations)

*  application (layer: Content)
*/    
    static const YdbStr ERROR_TYPE;
    static const YdbStr TRANSPORT;
    // RPC allready defined above
    static const YdbStr PROTOCOL;
    static const YdbStr APPLICATION;
/*
error-tag:  Contains a string identifying the error condition.  See
Appendix A for allowed values.
*/
    static const YdbStr ERROR_TAG;
/*
error-severity:  Contains a string identifying the error severity, as
determined by the device.  One of:

*  error

*  warning

Note that there are no <error-tag> values defined in this document
that utilize the "warning" enumeration.  This is reserved for
future use.
*/    
    static const YdbStr ERROR_SEVERITY;
    static const YdbStr ERROR;
    static const YdbStr WARNING;
/*
error-app-tag:  Contains a string identifying the data-model-specific
or implementation-specific error condition, if one exists.  This
element will not be present if no appropriate application error-
tag can be associated with a particular error condition.  If a
data-model-specific and an implementation-specific error-app-tag
both exist, then the data-model-specific value MUST be used by the
server.
 */
    static const YdbStr ERROR_APP_TAG;
/*
error-path:  Contains the absolute XPath [W3C.REC-xpath-19991116]
expression identifying the element path to the node that is
associated with the error being reported in a particular
<rpc-error> element.  This element will not be present if no
appropriate payload element or datastore node can be associated
with a particular error condition.

The XPath expression is interpreted in the following context:

*  The set of namespace declarations are those in scope on the
<rpc-error> element.

*  The set of variable bindings is empty.

*  The function library is the core function library.

The context node depends on the node associated with the error
being reported:

*  If a payload element can be associated with the error, the
context node is the rpc request's document node (i.e., the
<rpc> element).

*  Otherwise, the context node is the root of all data models,
i.e., the node that has the top-level nodes from all data
models as children.

*/
    static const YdbStr ERROR_PATH;
/*
error-message:  Contains a string suitable for human display that
describes the error condition.  This element will not be present
if no appropriate message is provided for a particular error
condition.  This element SHOULD include an "xml:lang" attribute as
defined in [W3C.REC-xml-20001006] and discussed in [RFC3470].
 */
    static const YdbStr ERROR_MESSAGE;
/*
error-info:  Contains protocol- or data-model-specific error content.
This element will not be present if no such error content is
provided for a particular error condition.  The list in Appendix A
defines any mandatory error-info content for each error.  After
any protocol-mandated content, a data model definition MAY mandate
that certain application-layer error information be included in
the error-info container.  An implementation MAY include
additional elements to provide extended and/or implementation-
specific debugging information.
*/
    static const YdbStr ERROR_INFO;
};

#endif
