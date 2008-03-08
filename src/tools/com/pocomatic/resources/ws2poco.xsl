<xsl:transform version  = '1.0'
                xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
                                                                                
<!--
                                                                                
   ws-application to poco-app-context transform template.

   Copyright Pocomatic LLC. 2006, All Rights Reserved.
   Author: Ke Jin <kejin@pocomatic.com>
                                                                                
-->

<xsl:output doctype-system =
       "http://www.pocomatic.com/poco-application-context.dtd"/>
<xsl:output method="xml" encoding="UTF-8"/>

<!-- ensure recursive template match -->
<xsl:template match="*|/">
  <xsl:apply-templates/>
</xsl:template>

<!-- copy all unspecified nodes -->
<xsl:template match="*">
  <xsl:copy>
    <xsl:copy-of select="@*"/>
    <xsl:apply-templates/>
  </xsl:copy>
</xsl:template>

<xsl:template match="/ws-application">
  <poco-application-context>
    <xsl:comment> transformed context </xsl:comment>
    <header>#include "wshelper.h"</header>
    <xsl:apply-templates/>
  </poco-application-context>
</xsl:template>

<xsl:template match="web-client">
  <!-- the web client init -->
  <bean class="POCO_WSClient"
        factory-method="POCO_WSHelper::WS_client_init"
        singleton="true"
        lazy-init="false"
        destroy-method="POCO_WSHelper::WS_client_term">
    <method-arg type="string">
      <xsl:if test="@config">
        <xsl:attribute name="value">
          <xsl:value-of select="@config"/>
        </xsl:attribute>
      </xsl:if>
    </method-arg>
  </bean>
</xsl:template>

<xsl:template match="web-server">
  <xsl:variable name="server-id">
    <xsl:if test="@id">
      <xsl:value-of select="@id"/>
    </xsl:if>
    <xsl:if test="not(@id)">
      <xsl:value-of select="concat('pocomatic.dispatcher.', generate-id(.))"/>
    </xsl:if>
  </xsl:variable>

  <!-- the web-server bean -->
  <xsl:comment> transformed from the &lt;web-server&gt; element </xsl:comment>
  <bean class="POCO_WSServer"
        factory-method="POCO_WSHelper::WS_init"
        singleton="true"
        lazy-init="false"
        destroy-method="POCO_WSHelper::WS_term">
    <xsl:copy-of select="@label"/>

    <xsl:attribute name="id">
       <xsl:value-of select="$server-id"/>
    </xsl:attribute>

    <method-arg type="string">
       <xsl:if test="@config">
         <xsl:attribute name="value"><xsl:value-of select="@config"/></xsl:attribute>
       </xsl:if>
    </method-arg>
  </bean>

  <xsl:if test="@id">
    <bean class="void"
          factory-method="POCO_WSHelper::WS_run"
          singleton="true"
          lazy-init="true">
      <xsl:attribute name="id">
        <xsl:value-of select="concat('pocomatic.dispatcher:', @id)"/>
      </xsl:attribute>
    </bean>
  </xsl:if>

  <!-- objects and child POAs after it -->
  <xsl:apply-templates select="endpoint"/>
</xsl:template>

<xsl:template match="endpoint">
  <xsl:comment> transformed from an &lt;endpoint&gt; element </xsl:comment>
  <xsl:variable name="id">
    <xsl:if test="@id">
      <xsl:value-of select="@id"/>
    </xsl:if>
    <xsl:if test="not(@id)">
      <xsl:value-of select="concat('pocomatic.object.', generate-id(.))"/>
    </xsl:if>
  </xsl:variable>
 
  <bean class="void"
        factory-method="POCO_WSHelper::export_service"
	lazy-init="false" 
        singleton="true">
    <xsl:copy-of select="@label"/>

    <xsl:if test="@id">
      <xsl:attribute name="id"><xsl:value-of select="$id"/></xsl:attribute>
    </xsl:if>

    <!-- first argument is the server -->
    <method-arg type="bean" class="POCO_WSServer">
      <xsl:if test="../@id">
        <xsl:attribute name="ref"><xsl:value-of select="../@id"/></xsl:attribute>
      </xsl:if>
      <xsl:if test="not(../@id)">
        <xsl:attribute name="ref">
          <xsl:value-of select="concat('pocomatic.dispatcher.', generate-id(..))"/>
        </xsl:attribute>
      </xsl:if>
    </method-arg>

    <!-- the second argument is service instance -->
    <method-arg type="bean" class="POCO_WSSkel">
      <xsl:copy-of select="bean|ref"/>
    </method-arg>

    <!-- the third argument is service name -->
    <method-arg type="string">
      <xsl:if test="@name">
        <xsl:attribute name="value"><xsl:value-of select="@name"/></xsl:attribute>
      </xsl:if>
    </method-arg>
    
    <!-- the fourth argument is wsdl file name -->
    <method-arg type="string">
      <xsl:if test="@wsdl">
        <xsl:attribute name="value">
          <xsl:value-of select="@wsdl"/>
        </xsl:attribute>
      </xsl:if>
    </method-arg>

    <!-- the fifth argument is the dispatch name (preserved in this releation) -->
    <method-arg type="string">
      <xsl:if test="@dispatcher">
        <xsl:attribute name="value">
          <xsl:value-of select="@dispatcher"/>
        </xsl:attribute>
      </xsl:if>
    </method-arg>
  </bean>
</xsl:template>

</xsl:transform>
