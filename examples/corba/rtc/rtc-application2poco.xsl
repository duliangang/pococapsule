<xsl:transform version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
                                                                                
<!--
	rtc-application to poco transformation stylesheet.

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
<xsl:template match="/rtc-application">
  <poco-application-context>
    <xsl:apply-templates/>
  </poco-application-context>
</xsl:template>

<xsl:template match="component">
  <bean self-destroy-method="finalize">
    <xsl:copy-of select="@class|@id|@lazy-init"/>
    <xsl:apply-templates select="@name"/>
    <ioc method="initialize"/>
    <xsl:apply-templates select="uses"/>
  </bean>

  <xsl:apply-templates select="listens"/>
</xsl:template>

<xsl:template match="component/@name">
  <method-arg type="string">
    <xsl:attribute name="value">
      <xsl:value-of select="."/>
    </xsl:attribute>
  </method-arg>
</xsl:template>

<xsl:template match="component/uses">
  <ioc>
     <xsl:attribute name="method">
       <xsl:value-of select="concat('connect_', @receptacle)"/>
     </xsl:attribute>
     <method-arg>
       <xsl:if test="@facet">
         <bean>
           <xsl:attribute name="class">
             <xsl:value-of select="@type"/>
           </xsl:attribute>
           <xsl:attribute name="factory-bean">
             <xsl:value-of select="@provider"/>
           </xsl:attribute>
           <xsl:attribute name="factory-method">
             <xsl:value-of select="concat('provide_', @facet)"/>
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

<xsl:template match="listens">
  <bean class="void" lazy-init="false">
     <xsl:attribute name="factory-method">
       <xsl:value-of select="concat('connect_', @receptacle)"/>
     </xsl:attribute>
     <xsl:attribute name="factory-bean">
       <xsl:value-of select="@emitter"/>
     </xsl:attribute>
     <xsl:attribute name="destroy-method">
       <xsl:value-of select="concat('disconnect_', @receptacle)"/>
     </xsl:attribute>
     <xsl:attribute name="destroy-bean">
       <xsl:value-of select="@emitter"/>
     </xsl:attribute>
     <method-arg>
       <xsl:if test="@sink">
         <bean>
           <xsl:attribute name="class">
             <xsl:value-of select="@type"/>
           </xsl:attribute>
           <xsl:attribute name="factory-bean">
             <xsl:value-of select="../@id"/>
           </xsl:attribute>
           <xsl:attribute name="factory-method">
             <xsl:value-of select="concat('get_consumer_', @sink)"/>
           </xsl:attribute>
         </bean>
       </xsl:if>
       <xsl:if test="not(@sink)">
         <xsl:attribute name="ref">
           <xsl:value-of select="../@id"/>
         </xsl:attribute>
       </xsl:if>
     </method-arg>
  </bean>
</xsl:template>

</xsl:transform>
