#ifndef _NETCONF_TAGS_HH_
#define _NETCONF_TAGS_HH_

#include "YdbStr.hh"

class NetconfIdentities
{
public:
    static const YdbStr NETCONF_XMLNS;
    static const YdbStr DATA;
    static const YdbStr CONFIG;
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
    static const YdbStr BAD_ELEMENT;
    static const YdbStr OPERATION_NOT_SUPPORTED;
    static const YdbStr OPERATION_FAILED;
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


    /*
      error-option:  The <error-option> element has one of the following
      values:

      stop-on-error:  Abort the <edit-config> operation on first
        error.  This is the default error-option.
	
      continue-on-error:  Continue to process configuration data on
         error; error is recorded, and negative response is generated
	 if any errors occur.

      rollback-on-error:  If an error condition occurs such that an
        error severity <rpc-error> element is generated, the server
	will stop processing the <edit-config> operation and restore
	the specified configuration to its complete state at the
	start of this <edit-config> operation.  This option requires
	the server to support the :rollback-on-error capability
	described in Section 8.5.
     */
    static const YdbStr ERROR_OPTION;
    static const YdbStr STOP_ON_ERROR;
    static const YdbStr CONTINUE_ON_ERROR;
    static const YdbStr ROLLBACK_ON_ERROR;
};

/*
Appendix A.  NETCONF Error List

This section is normative.

For each error-tag, the valid error-type and error-severity values
are listed, together with any mandatory error-info, if any.

error-tag:      in-use
error-type:     protocol, application
error-severity: error
error-info:     none
Description:    The request requires a resource that already is in
use.

error-tag:      invalid-value
error-type:     protocol, application
error-severity: error
error-info:     none
Description:    The request specifies an unacceptable value for one
                or more parameters.

error-tag:      too-big
error-type:     transport, rpc, protocol, application
error-severity: error
error-info:     none
Description:    The request or response (that would be generated) is
                too large for the implementation to handle.

error-tag:      missing-attribute
error-type:     rpc, protocol, application
error-severity: error
error-info:     <bad-attribute> : name of the missing attribute
                <bad-element> : name of the element that is supposed
                to contain the missing attribute
Description:    An expected attribute is missing.

error-tag:      bad-attribute
error-type:     rpc, protocol, application
error-severity: error
error-info:     <bad-attribute> : name of the attribute w/ bad value
                <bad-element> : name of the element that contains
                the attribute with the bad value
Description:    An attribute value is not correct; e.g., wrong type,
out of range, pattern mismatch.

error-tag:      unknown-attribute
error-type:     rpc, protocol, application
error-severity: error
error-info:     <bad-attribute> : name of the unexpected attribute
                <bad-element> : name of the element that contains
the unexpected attribute
Description:    An unexpected attribute is present.

error-tag:      missing-element
error-type:     protocol, application
error-severity: error
error-info:     <bad-element> : name of the missing element
Description:    An expected element is missing.

error-tag:      bad-element
error-type:     protocol, application
error-severity: error
error-info:     <bad-element> : name of the element w/ bad value
Description:    An element value is not correct; e.g., wrong type,
                out of range, pattern mismatch.

error-tag:      unknown-element
error-type:     protocol, application
error-severity: error
error-info:     <bad-element> : name of the unexpected element
Description:    An unexpected element is present.

error-tag:      unknown-namespace
error-type:     protocol, application
error-severity: error
error-info:     <bad-element> : name of the element that contains
the unexpected namespace
<bad-namespace> : name of the unexpected namespace
Description:    An unexpected namespace is present.

error-tag:      access-denied
error-type:     protocol, application
error-severity: error
error-info:     none
Description:    Access to the requested protocol operation or
                data model is denied because authorization failed.

error-tag:      lock-denied
error-type:     protocol
error-severity: error
error-info:     <session-id> : session ID of session holding the
                requested lock, or zero to indicate a non-NETCONF
                entity holds the lock
Description:    Access to the requested lock is denied because the
                lock is currently held by another entity.

error-tag:      resource-denied
error-type:     transport, rpc, protocol, application
error-severity: error
error-info:     none
Description:    Request could not be completed because of
                insufficient resources.

error-tag:      rollback-failed
error-type:     protocol, application
error-severity: error
error-info:     none
Description:    Request to roll back some configuration change (via
                rollback-on-error or <discard-changes> operations)
                was not completed for some reason.

error-tag:      data-exists
error-type:     application
error-severity: error
error-info:     none
Description:    Request could not be completed because the relevant
                data model content already exists.  For example,
                a "create" operation was attempted on data that
                already exists.

error-tag:      data-missing
error-type:     application
error-severity: error
error-info:     none
Description:    Request could not be completed because the relevant
                data model content does not exist.  For example,
                a "delete" operation was attempted on
                data that does not exist.

error-tag:      operation-not-supported
error-type:     protocol, application
error-severity: error
error-info:     none
Description:    Request could not be completed because the requested
                operation is not supported by this implementation.

error-tag:      operation-failed
error-type:     rpc, protocol, application
error-severity: error
error-info:     none
Description:    Request could not be completed because the requested
                operation failed for some reason not covered by
                any other error condition.

error-tag:      partial-operation
error-type:     application
error-severity: error
error-info:     <ok-element> : identifies an element in the data
                model for which the requested operation has been
                completed for that node and all its child nodes.
                This element can appear zero or more times in the
                <error-info> container.

<err-element> : identifies an element in the data
                model for which the requested operation has failed
                for that node and all its child nodes.
                This element can appear zero or more times in the
<error-info> container.

<noop-element> : identifies an element in the data
                model for which the requested operation was not
                attempted for that node and all its child nodes.
                This element can appear zero or more times in the
<error-info> container.

Description:    This error-tag is obsolete, and SHOULD NOT be sent
                by servers conforming to this document.
                Some part of the requested operation failed or was
                not attempted for some reason.  Full cleanup has
                not been performed (e.g., rollback not supported)
                by the server.  The error-info container is used
                to identify which portions of the application
                data model content for which the requested operation
                has succeeded (<ok-element>), failed (<bad-element>),
                or not been attempted (<noop-element>).

error-tag:      malformed-message
error-type:     rpc
error-severity: error
error-info:     none
Description:    A message could not be handled because it failed to
                be parsed correctly.  For example, the message is not
                well-formed XML or it uses an invalid character set.

                This error-tag is new in :base:1.1 and MUST NOT be
                sent to old clients.

*/

#endif
