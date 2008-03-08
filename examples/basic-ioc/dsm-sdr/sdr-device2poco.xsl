<xsl:transform version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
                                                                                
<!--
	A user defined SDR application assembly schema to PocoCapsule/C++
	core schema transformation stylesheet.

	Copyright 2007 by Pocomatic Software LLC, All Rights Reserved.

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

<!-- ext-appctxt is mapped to poco-application-context -->
<xsl:template match="/sdr-device">
  <poco-application-context>
    <xsl:apply-templates/>
  </poco-application-context>
</xsl:template>

<xsl:template match="component">
  <bean>
    <xsl:copy-of select="@class|@id|@lazy-init"/>
    <xsl:apply-templates select="@name"/>
    <xsl:apply-templates/>
  </bean>
</xsl:template>

<xsl:template match="component/@name">
  <method-arg type="string">
    <xsl:attribute name="value">
      <xsl:value-of select="."/>
    </xsl:attribute>
  </method-arg>
</xsl:template>

<xsl:template match="component/wire">
  <ioc>
     <xsl:attribute name="method">
       <xsl:value-of select="@receptacle"/>
     </xsl:attribute>
     <method-arg>
       <xsl:if test="@facet">
         <bean class="SDR::PushPort">
           <xsl:attribute name="factory-bean">
             <xsl:value-of select="@provider"/>
           </xsl:attribute>
           <xsl:attribute name="factory-method">
             <xsl:value-of select="@facet"/>
           </xsl:attribute>
         </bean>
       </xsl:if>
       <xsl:if test="not(@facet)">
         <xsl:attribute name="ref">
           <xsl:value-of select="@provider"/>
         </xsl:attribute>
       </xsl:if>
     </method-arg>
  </ioc>
</xsl:template>

</xsl:transform>
