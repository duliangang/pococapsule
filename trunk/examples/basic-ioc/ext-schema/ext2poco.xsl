<xsl:transform version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

<!--
	A DSL schema that extend the PocoCapsule/C++ core schema to 
	support <map> and <property> elements.

	Copyright 2006 by Pocomatic Software LLC, All Rights Reserved.

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
<xsl:template match="/ext-appctxt">
  <poco-application-context>
    <xsl:copy-of select="@id"/>
    <header>#include &lt;map&gt;</header>
    <xsl:apply-templates/>
  </poco-application-context>
</xsl:template>

<!-- a bean's property is mapped to an post-instantiation ioc -->
<xsl:template match="bean/property">
  <ioc>
     <xsl:attribute name="method">
       <xsl:value-of select="@name"/>
     </xsl:attribute>
     <method-arg>
       <xsl:apply-templates/>
       <xsl:copy-of select="@*[not(name()='name')]"/>
     </method-arg>
  </ioc>
</xsl:template>

<!-- a map arg is mapped to a bean -->
<xsl:template match="map">
  <bean destroy-method="delete">
    <xsl:copy-of select="@id"/>
    <xsl:if test="not(@id)">
      <xsl:attribute name="singleton">false</xsl:attribute>
    </xsl:if>
    <xsl:attribute name="class">
      <xsl:value-of select="concat('std::map{std::string, ', @class, '*}')"/>
    </xsl:attribute>
    <xsl:apply-templates select="entry"/>
  </bean>
</xsl:template>

<!-- a map's entry is mapped to a post-instantiation ioc -->
<xsl:template match="map/entry">
  <ioc method="add_to_map" target="none">
    <method-arg pass="deref"><this-bean/></method-arg>
    <method-arg type="string">
      <xsl:attribute name="value">
        <xsl:value-of select="@key"/>
      </xsl:attribute>
    </method-arg>
    <method-arg>
      <xsl:copy-of select="@ref"/>
      <xsl:apply-templates/>
    </method-arg>
  </ioc>
</xsl:template>

</xsl:transform>
