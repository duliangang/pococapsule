<xsl:transform version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
                                                                                
<!--
	Copyright 2007, 2008 by Pocomatic Software LLC, All Rights Reserved.

	A higher order transform (HOT) stylesheet that transform a poco-dsl
	stylesheet to a XSLT stylesheet.

	Author: Ke Jin <kejin@pocomatic.com>
-->

<!-- ensure recursive template match -->
<xsl:template match="*|/">
  <xsl:apply-templates/>
</xsl:template>

<!-- copy all unspecified nodes -->
<xsl:template match="*">
  <xsl:copy>
    <xsl:apply-templates select="*|@*"/>
  </xsl:copy>
</xsl:template>

<xsl:template match="@*">
  <xsl:element name="xsl:attribute">
    <xsl:attribute name="name"><xsl:value-of select="name(.)"/></xsl:attribute> 
    <xsl:text><xsl:value-of select="."/></xsl:text>
  </xsl:element>
</xsl:template>

<xsl:template match="@*[starts-with(., '#')]">
  <xsl:element name="xsl:attribute">
    <xsl:attribute name="name"><xsl:value-of select="name(.)"/></xsl:attribute>
    <xsl:element name="xsl:value-of">
      <xsl:attribute name="select">
        <xsl:value-of select="substring(., 2)"/>
      </xsl:attribute>
    </xsl:element>
  </xsl:element>
</xsl:template>

<xsl:template match="bean/@class|method-arg/@value|item/@value[starts-with(., '@')]">
  <xsl:element name="xsl:attribute">
    <xsl:attribute name="name">
      <xsl:value-of select="name(.)"/>
    </xsl:attribute>
    <xsl:element name="xsl:value-of">
      <xsl:attribute name="select">
        <xsl:value-of select="."/>
      </xsl:attribute>
    </xsl:element>
  </xsl:element>
</xsl:template>

<xsl:template match="poco-dsl-transform">
  <xsl:element name="xsl:transform">
    <xsl:attribute name="version">1.0</xsl:attribute>
    <xsl:element name="xsl:output">
      <xsl:attribute name="doctype-system">http://www.pocomatic.com/poco-application-context.dtd</xsl:attribute>
    </xsl:element>

    <xsl:element name="xsl:output">
      <xsl:attribute name="method">xml</xsl:attribute>
      <xsl:attribute name="encoding">UTF-8</xsl:attribute>
    </xsl:element>

    <xsl:element name="xsl:template">
      <xsl:attribute name="match">*|/</xsl:attribute>
      <xsl:element name="xsl:apply-templates"/>
    </xsl:element>

    <xsl:element name="xsl:template">
      <xsl:attribute name="match">*</xsl:attribute>
      <xsl:element name="xsl:copy">
        <xsl:element name="xsl:copy-of">
          <xsl:attribute name="select">@*</xsl:attribute>
        </xsl:element>
        <xsl:element name="xsl:apply-templates"/>
      </xsl:element>
    </xsl:element>

    <xsl:apply-templates select="*"/>
  </xsl:element>
</xsl:template>

<xsl:template match="dsl-template">
  <xsl:element name="xsl:template">
    <xsl:copy-of select="@*"/>
    <xsl:apply-templates/>
  </xsl:element>
</xsl:template>

<xsl:template match="dsl-apply-templates">
  <xsl:element name="xsl:apply-templates">
    <xsl:copy-of select="@*"/>
    <xsl:apply-templates/>
  </xsl:element>
</xsl:template>

</xsl:transform>
