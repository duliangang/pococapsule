<xsl:transform version  = '1.0'
		xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
                                                                                
<!--
   OASIS OCSA <composite> to poco-app-context transform template.

   Copyright Pocomatic LLC. 2006, All Rights Reserved.
   Author: Ke Jin <kejin@pocomatic.com>
                                                                                
-->

<xsl:output doctype-system =
       "http://www.pocomatic.com/poco-application-context.dtd"/>
<xsl:output method="xml" encoding="UTF-8"/>

<xsl:key name="composite-property-idx" match="composite/property" use="@name"/>

<xsl:template match="composite">
  <xsl:comment> transformed from a &lt;composite&gt; element </xsl:comment>
  <poco-application-context>
    <xsl:if test="@name">
      <xsl:attribute name="id"><xsl:value-of select="@name"/></xsl:attribute>
    </xsl:if>
    <header>#include "wshelper.h"</header>
    <xsl:apply-templates select="component|service|property|reference|wire"/>

    <xsl:comment> implicit server reference </xsl:comment>
    <bean id="pocomatic.ws-server-id"
          class="POCO_WSServer"
          destroy-method="POCO_WSHelper::WS_term"
          factory-method="POCO_WSHelper::WS_init">
        <method-arg type="string"/>
    </bean>

    <xsl:comment> implicit client runtime </xsl:comment>
    <bean id="pocomatic.ws-client-id"
          class="POCO_WSClient"
          destroy-method="POCO_WSHelper::WS_client_term"
          factory-method="POCO_WSHelper::WS_client_init">
        <method-arg type="string"/> 
    </bean>
  </poco-application-context>
</xsl:template>

<xsl:template match="composite/service">
  <xsl:comment> transformed from a composite &lt;service&gt; element </xsl:comment>
  <xsl:for-each select="binding.ws">
    <xsl:if test="not(../reference)">
      <transform-error message="A &lt;service&gt; element missing a &lt;reference&gt; child element!"/>
    </xsl:if>
    <xsl:variable name="skel-ref">
      <xsl:value-of select="../reference"/>
    </xsl:variable>
    <xsl:variable name="var1">
      <xsl:value-of select="substring-before(substring-after(@port, '#wsdl.endpoint('), ')')"/>
    </xsl:variable>
    <xsl:variable name="var2">
      <xsl:value-of select="substring-before($var1, '/')"/>
    </xsl:variable>
    <xsl:variable name="service-name">
      <xsl:choose>
      <xsl:when test="$var2 and string-length($var2) != 0">
        <xsl:value-of select="$var2"/>
      </xsl:when>
      <xsl:when test="$var1 and string-length($var1) != 0">
        <xsl:value-of select="$var1"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:message>+++ Warning: can't determine service name</xsl:message>
      </xsl:otherwise>
    </xsl:choose>
    </xsl:variable>
    <xsl:if test="starts-with(@port, 'http://') or starts-with(@port, 'https://')">
      <xsl:message>+++ Warning: service transport binding not supported</xsl:message>
    </xsl:if>
    <bean class="void"
          lazy-init="false"
          factory-method="POCO_WSHelper::export_service">
          <method-arg class="POCO_WSServer" ref="pocomatic.ws-server-id"/>
          <method-arg class="POCO_WSSkel">
            <xsl:attribute name="ref"><xsl:value-of select="$skel-ref"/></xsl:attribute> 
          </method-arg>
          <method-arg type="string">
            <xsl:attribute name="value"><xsl:value-of select="$service-name"/></xsl:attribute>
          </method-arg>
          <method-arg type="string"/>
          <method-arg type="string"/>
    </bean>
  </xsl:for-each>
</xsl:template>

<xsl:template match="composite/component">
  <xsl:comment> transformed from a &lt;component&gt; element </xsl:comment>
  <bean>
    <xsl:attribute name="id"><xsl:value-of select="@name"/></xsl:attribute>
    <xsl:attribute name="class">
      <xsl:value-of select="implementation.cpp/@class"/>
    </xsl:attribute>
    <xsl:apply-templates select="property|reference"/>
  </bean>
</xsl:template>

<xsl:template match="component/property">
  <xsl:variable name="prop-type">
    <xsl:choose>
    <xsl:when test="@source">
      <xsl:choose>
      <xsl:when test="starts-with(@source, '$')">
        <xsl:if test="not(key('composite-property-idx', substring-after(@source, '$')))">
          <transform-error>
            <xsl:attribute name="message">
            <xsl:value-of select="concat(
              'Fail to resolve the composite property with name &quot;', 
              substring-after(@source, '$'),
              '&quot; that is referred by the &quot;source&quot; attribute of a component property.')"/> 
            </xsl:attribute>
          </transform-error>
        </xsl:if>
        <xsl:value-of select="key('composite-property-idx', substring-after(@source, '$'))/@type"/>
      </xsl:when>
      <xsl:otherwise>
        <transform-error message="A property &quot;source&quot; attribute's value should started with '$'!"/>
      </xsl:otherwise>
      </xsl:choose>
    </xsl:when>
    <xsl:when test="@type">
      <xsl:value-of select="@type"/>
    </xsl:when>
    <xsl:otherwise>
      <transform-error message="A component's property missing type or source attribute!"/>
    </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <xsl:variable name="prop-value">
    <xsl:choose>
    <xsl:when test="@source">
      <xsl:value-of select="key('composite-property-idx', substring-after(@source, '$'))"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="."/>
    </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <xsl:comment> transformed from a component &lt;property&gt; </xsl:comment>
  <ioc>
    <xsl:attribute name="method">
      <xsl:value-of select="@name"/>
    </xsl:attribute>
    <method-arg>
      <xsl:attribute name="type">
        <xsl:choose>
        <xsl:when test="not($prop-type)">
          <xsl:value-of select="'string'"/>
        </xsl:when>
        <xsl:when test="$prop-type='xsd:boolean'">
          <xsl:value-of select="'short'"/>
        </xsl:when>
        <xsl:when test="$prop-type='xsd:string'">
          <xsl:value-of select="'string'"/>
        </xsl:when>
        <xsl:when test="$prop-type='xsd:short'">
          <xsl:value-of select="'short'"/>
        </xsl:when>
        <xsl:when test="$prop-type='xsd:unsignedShort'">
          <xsl:value-of select="'ushort'"/>
        </xsl:when>
        <xsl:when test="$prop-type='xsd:int'">
          <xsl:value-of select="'long'"/>
        </xsl:when>
        <xsl:when test="$prop-type='xsd:unsignedInt'">
          <xsl:value-of select="'ulong'"/>
        </xsl:when>
        <xsl:when test="$prop-type='xsd:byte'">
          <xsl:value-of select="'byte'"/>
        </xsl:when>
        <xsl:when test="$prop-type='xsd:float'">
          <xsl:value-of select="'float'"/>
        </xsl:when>
        <xsl:when test="$prop-type='xsd:double'">
          <xsl:value-of select="'double'"/>
        </xsl:when>
        <xsl:when test="$prop-type='xsd:unsignedByte'">
          <xsl:value-of select="'byte'"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:if test="$prop-type and string-length($prop-type) != 0">
            <transform-error>
              <xsl:attribute name="message">
              <xsl:value-of select="concat('The specified property type &quot;', 
                                   $prop-type, '&quot; is not supported')"/>
              </xsl:attribute>
             </transform-error>
          </xsl:if>
        </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:choose>
        <xsl:when test="$prop-type='xsd:boolean'">
          <xsl:choose>
          <xsl:when test="$prop-value='true'">
            <xsl:value-of select='1'/>
          </xsl:when>
          <xsl:when test="$prop-value='false'">
            <xsl:value-of select='0'/>
          </xsl:when>
          <xsl:otherwise>
            <transform-error>
              <xsl:attribute name="message">
              <xsl:value-of select="concat('Invalide boolean type value &quot;', 
                                    $prop-value, '&quot;')"/>
              </xsl:attribute>
            </transform-error>
          </xsl:otherwise>
          </xsl:choose>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$prop-value"/>
        </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </method-arg>
  </ioc>
</xsl:template>

<xsl:template match="component/reference">
  <xsl:comment> transformed from a component &lt;reference&gt; element </xsl:comment>
  <ioc>
    <xsl:attribute name="method">
      <xsl:value-of select="@name"/>
    </xsl:attribute>
    <method-arg>
      <xsl:attribute name="ref">
        <xsl:value-of select="."/>
      </xsl:attribute>
    </method-arg>
  </ioc>
</xsl:template>

<xsl:template match="composite/reference">
  <xsl:comment> transformed from a composite &lt;reference&gt; element </xsl:comment>
  <xsl:variable name="stub-class-name">
    <xsl:choose>
    <xsl:when test="interface.cpp/@interface">
        <xsl:value-of select="interface.cpp/@interface"/>
    </xsl:when>
    <xsl:when test="interface.wsdl/@interface">
      <xsl:value-of select=
        "substring-before(substring-after(interface.wsdl/@interface, '#wsdl.interface('), ')')"/>
    </xsl:when>
    </xsl:choose>
  </xsl:variable>
  <bean factory-method="POCO_WSHELPER_WS_BINDING">
    <xsl:if test="not($stub-class-name) or string-length($stub-class-name) = 0">
      <transform-error message="A &lt;reference&gt; element at &lt;composite&gt; level is incomplete!"/>
    </xsl:if>
    <xsl:attribute name="class">
      <xsl:value-of select="$stub-class-name"/>
    </xsl:attribute>
    <xsl:if test="not(@name)">
      <transform-error message="A &lt;reference&gt; element at &lt;composite&gt; level missing name attribute!"/>
    </xsl:if>
    <xsl:attribute name="id">
      <xsl:value-of select="@name"/>
    </xsl:attribute>
    <method-arg class="POCO_WSClient" ref="pocomatic.ws-client-id"/>
    <method-arg>
      <xsl:attribute name="ref">
        <xsl:value-of select="concat('pocomatic.poco_bean_id:', @name)"/>
      </xsl:attribute>
      <xsl:attribute name="class">
        <xsl:value-of select="$stub-class-name"/>
      </xsl:attribute>
    </method-arg>
    <method-arg type="string">
      <xsl:if test="binding.ws/@port">
        <xsl:attribute name="value">
          <xsl:choose>
          <xsl:when test="substring-before(binding.ws/@port, '#wsdl.endpoint')">
            <xsl:value-of select="substring-before(binding.ws/@port, '#wsdl.endpoint')"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="binding.ws/@port"/>
          </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:if>
    </method-arg>
  </bean>

  <bean destroy-method="delete">
    <xsl:attribute name="class">
      <xsl:value-of select="$stub-class-name"/>
    </xsl:attribute>
    <xsl:attribute name="id">
      <xsl:value-of select="concat('pocomatic.poco_bean_id:', @name)"/>
    </xsl:attribute>
  </bean>
</xsl:template>

<xsl:template match="composite/wire">
  <xsl:variable name="target-id">
    <xsl:value-of select="substring-before(target.uri, '/')"/>
  </xsl:variable>
  <xsl:variable name="target-port">
    <xsl:value-of select="substring-after(target.uri, '/')"/>
  </xsl:variable>
  <xsl:if test="not($target-port) or string-length($target-port) = 0">
    <transform-error message="Invalid target.uri in &lt;wire&gt;"/>
  </xsl:if> 
  <xsl:comment> transformed from a &lt;wire&gt; element </xsl:comment>
  <bean class="void" lazy-init="false">
    <xsl:attribute name="factory-bean">
      <xsl:value-of select="$target-id"/>
    </xsl:attribute>
    <xsl:attribute name="factory-method">
      <xsl:value-of select="$target-port"/>
    </xsl:attribute>
    <method-arg>
      <xsl:attribute name="ref">
        <xsl:value-of select="source.uri"/>
      </xsl:attribute>
    </method-arg>
  </bean>
</xsl:template>

</xsl:transform>
