<xsl:transform version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
                                                                                
<!--
	A user defined gps model schema to poco core schema transformation
	style sheet.

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

<!-- gps-device is mapped to poco-application-context -->
<xsl:template match="/gps-device">
  <poco-application-context>
    <xsl:apply-templates/>
    <load library="./reflx.$dll"/>
  </poco-application-context>
</xsl:template>

<xsl:template match="tick-generator">
  <xsl:apply-templates select="@use" mode="load"/>
  <bean destroy-method="delete" lazy-init="false">
    <xsl:apply-templates select="@use" mode="class"/>
    <method-arg type="short">
      <xsl:attribute name="value">
        <xsl:value-of select="@count"/>
      </xsl:attribute>
    </method-arg>
    <method-arg type="short">
      <xsl:attribute name="value">
        <xsl:value-of select="@interval"/>
      </xsl:attribute>
    </method-arg>
    <ioc method="subscribe">
      <method-arg ref="gps-locator"/>
    </ioc>
    <ioc method="start"/>
  </bean>
</xsl:template>

<xsl:template match="gps-locator">
  <xsl:apply-templates select="@use" mode="load"/>
  <bean id="gps-locator" destroy-method="delete">
    <xsl:apply-templates select="@use" mode="class"/>
    <xsl:apply-templates select="../navigation-display" mode="subscribe"/>
  </bean>
</xsl:template>

<xsl:template match="navigation-display">
  <xsl:apply-templates select="@use" mode="load"/>
  <bean destroy-method="delete">
    <xsl:attribute name="id">
      <xsl:value-of select="concat('nav-display-', generate-id())"/>
    </xsl:attribute>
    <xsl:apply-templates select="@use" mode="class"/>
    <method-arg ref="gps-locator"/>
  </bean>
</xsl:template>

<xsl:template match="@use" mode="load">
  <load>
    <xsl:attribute name="library">
      <xsl:value-of select="concat('./', ., '.$dll')"/>
    </xsl:attribute>
  </load>
</xsl:template>

<xsl:template match="@use" mode="class">
  <xsl:attribute name="class"><xsl:value-of select="."/></xsl:attribute>
</xsl:template>

<xsl:template match="navigation-display" mode="subscribe">
  <ioc method="subscribe">
    <method-arg>
      <xsl:attribute name="ref">
        <xsl:value-of select="concat('nav-display-', generate-id())"/>
      </xsl:attribute>
    </method-arg>
  </ioc>
</xsl:template>

</xsl:transform>
