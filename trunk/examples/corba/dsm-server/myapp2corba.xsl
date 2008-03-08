<xsl:transform version  = '1.0' 
                xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

<!--

   An user defined domain specific language schema to poco corba 
   schema transformer example:

   my-server-application to poco corba schema transformer stylesheet.
   Copyright Pocomatic Software. 2006
   Author: Ke Jin

-->

<xsl:output method="xml" encoding="iso-8859-1"/>
<xsl:output doctype-system 
	= "http://www.pocomatic.com/corba-application-context.dtd"/>

<xsl:template match="/my-application-context">
  <xsl:processing-instruction name="xml-transform">
   type="text/xsl" href="http://www.pocomatic.com/corba2poco.xsl"
  </xsl:processing-instruction>

  <corba-application-context>
    <xsl:apply-templates select="header|load"/>

    <orb id="my-orb">
      <xsl:apply-templates/>
    </orb>
  </corba-application-context>
</xsl:template>

<xsl:template match="header|load">
  <xsl:copy-of select="."/>
</xsl:template>

<xsl:template match="home-with-object-map-only">
  <poa>
    <policies>
      <policy name="request-processing" value="use-aom-only"/>
    </policies>

    <xsl:apply-templates/>
  </poa>
</xsl:template>

<xsl:template match="home-with-object-default">
  <poa>
    <policies>
      <policy name="request-processing" value="use-default-servant"/>
    </policies>

    <xsl:apply-templates/>
  </poa>
</xsl:template>

<xsl:template match="home-with-object-locator">
  <poa>
    <policies>
      <policy name="request-processing" value="use-servant-manager"/>
      <policy name="servant-retention" value="non-retain"/>
    </policies>

    <xsl:apply-templates/>
  </poa>
</xsl:template>

<xsl:template match="home-with-object-activator">
  <poa>
    <policies>
      <policy name="request-processing" value="use-servant-manager"/>
      <policy name="servant-retention" value="retain"/>
    </policies>

    <xsl:apply-templates/>
  </poa>
</xsl:template>

<xsl:template match="object-activation">
  <object>
    <xsl:attribute name="uri"><xsl:value-of select="@uri"/></xsl:attribute>

    <xsl:apply-templates/>
  </object>
</xsl:template>

<xsl:template match="object-reference">
  <object>
    <xsl:attribute name="uri"><xsl:value-of select="@uri"/></xsl:attribute>
  </object>
</xsl:template>

<xsl:template match="object-activation/servant">
  <bean>
    <xsl:attribute name="class"><xsl:value-of select="@class"/></xsl:attribute>
    <method-arg type="string">
      <xsl:attribute name="value">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
    </method-arg>
  </bean>
</xsl:template>

<xsl:template match="home-with-object-default/servant">
  <property name="set_servant">
  <bean>
    <xsl:attribute name="class"><xsl:value-of select="@class"/></xsl:attribute>
    <method-arg type="string">
      <xsl:attribute name="value">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
    </method-arg>
  </bean>
  </property>
</xsl:template>

<xsl:template match="servant-locator">
  <property name="set_servant_manager">
      <bean>
        <xsl:attribute name="class"><xsl:value-of select="@class"/></xsl:attribute>
      </bean>
  </property>
</xsl:template>

<xsl:template match="servant-activator">
  <property name="set_servant_manager">
      <bean>
        <xsl:attribute name="class"><xsl:value-of select="@class"/></xsl:attribute>
      </bean>
  </property>
</xsl:template>

</xsl:transform>
