<xsl:transform version  = '1.0'
                xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
                                                                                
<!--
                                                                                
   jtrs-sca-lite softwareassembly to poco-app-context transform template.

   Copyright Pocomatic LLC. 2006, All Rights Reserved.
   Author: Ke Jin <kejin@pocomatic.com>
                                                                                
-->

<xsl:output doctype-system =
       "http://www.pocomatic.com/poco-application-context.dtd"/>
<xsl:output method="xml" encoding="UTF-8"/>

<xsl:template match="/softwareassembly|/deviceconfiguration">
  <poco-application-context>
    <header>#include "scahelper.h"</header>
    <xsl:comment> transformed context </xsl:comment>
    <xsl:apply-templates select="componentfiles"/>
    <xsl:apply-templates select="domainmanager"/>
    <xsl:apply-templates select="connections"/>
  </poco-application-context>
</xsl:template>

<xsl:template match="domainmanager">
  <bean class="void"
        singleton="true"
        lazy-init="false"
        factory-method="poco_sachelper_register_domain_manager">
    <method-arg class="*CORBA::Object_ptr">
      <bean class="*CORBA::Object_ptr"
            singleton="false"
            destroy-method="CORBA::release">
        <xsl:attribute name="factory-method">poco_scahelper::findby_ns</xsl:attribute>
        <method-arg type="string">
         <xsl:attribute name="value"><xsl:value-of select="namingservice/@name"/></xsl:attribute>
        </method-arg>
      </bean>
    </method-arg>
  </bean>
</xsl:template>

<xsl:template match="componentfiles">
  <xsl:apply-templates select="componentfile"/>
</xsl:template>

<xsl:template match="componentfile">
  <import>
    <xsl:attribute name="resource"><xsl:value-of select="./localfile/@name"/></xsl:attribute>
  </import>

  <bean class="void"
        factory-method="poco_scahelper::register_component_file">
    <method-arg type="string">
      <xsl:attribute name="value"><xsl:value-of select="@id"/></xsl:attribute>
    </method-arg>
    <method-arg type="string">
      <xsl:attribute name="value"><xsl:value-of select="./localfile/@name"/></xsl:attribute>
    </method-arg>

    <xsl:if test="@type">
      <method-arg type="string">
        <xsl:attribute name="value"><xsl:value-of select="@type"/></xsl:attribute>
      </method-arg>
    </xsl:if>
    <xsl:if test="not(@type)">
      <method-arg type="string"/>
    </xsl:if>
  </bean>
</xsl:template>

<xsl:template match="connections">
  <xsl:apply-templates select="connectinterface"/>
</xsl:template>

<xsl:template match="connectinterface">
  <bean class="void"
        factory-method="poco_scahelper::connect_interface"
        lazy-init="false">
     <xsl:apply-templates select="usesport"/>
     <xsl:apply-templates select="providesport"/>
     <xsl:if test="not(providesport)">
       <method-arg type="string"></method-arg>
       <method-arg class="*CORBA::Object_ptr">
         <xsl:apply-templates select="findby"/>
       </method-arg>
     </xsl:if>
     <method-arg type="string">
       <xsl:attribute name="value"><xsl:value-of select="@id"/></xsl:attribute>
     </method-arg>
  </bean>  
</xsl:template>

<xsl:template match="usesport">
  <method-arg type="string">
    <xsl:attribute name="value"><xsl:value-of select="usesidentifier"/></xsl:attribute>
  </method-arg>
  <method-arg class="*CORBA::Object_ptr">
    <xsl:apply-templates select="findby"/>
  </method-arg>
</xsl:template>

<xsl:template match="providesport">
  <method-arg type="string">
    <xsl:attribute name="value"><xsl:value-of select="providesidentifier"/></xsl:attribute>
  </method-arg>
  <method-arg class="*CORBA::Object_ptr">
    <xsl:apply-templates select="findby"/>
  </method-arg>
</xsl:template>

<xsl:template match="findby">
  <bean class="*CORBA::Object_ptr"
        destroy-method="CORBA::release"
        singleton="false"
        lazy-init="true" >
     <xsl:if test="namingservice">
       <xsl:attribute name="factory-method">poco_scahelper::findby_ns</xsl:attribute>
       <method-arg type="string">
         <xsl:attribute name="value"><xsl:value-of select="namingservice/@name"/></xsl:attribute>
       </method-arg>
     </xsl:if>

     <xsl:if test="./domainfinder">
       <xsl:attribute name="factory-method">poco_scahelper::findby_ds</xsl:attribute>
       <method-arg type="string">
         <xsl:attribute name="value"><xsl:value-of select="domainfinder/@type"/></xsl:attribute>
       </method-arg>
       <xsl:if test="domainfinder/@name">
         <method-arg type="string">
           <xsl:attribute name="value"><xsl:value-of select="domainfinder/@name"/></xsl:attribute>
         </method-arg>
       </xsl:if>
       <xsl:if test="not(domainfinder/@name)">
         <method-arg type="string"/>
       </xsl:if>
     </xsl:if>
  </bean>
</xsl:template>

</xsl:transform>
