<xsl:transform version	= '1.0'
		xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

<!--

   poco-application-context to spring-beans transformation: for SpringIDE.
   Contents not relevant to bean property and bean dependency may be filted out.
   Array elements are filted out also (might be added back in later enhanced release).

   Copyright Pocomatic LLC. 2006, All Rights Reserved.
   Author: Ke Jin <kejin@pocomatic.com>

-->

<xsl:output doctype-system =
       "http://www.springframework.org/dtd/spring-beans.dtd"/>
<xsl:output method="xml" encoding="UTF-8"/>

<xsl:template match="poco-application-context">
   <beans>
     <xsl:apply-templates select="bean"/>
   </beans>
</xsl:template>

<xsl:template match="bean">
  <bean>
    <xsl:copy-of select="@class|@factory-method|@factory-bean|@id|@depends-on"/>

    <xsl:apply-templates select="method-arg"/>
    <xsl:apply-templates select="ioc"/>
  </bean>
</xsl:template>

<xsl:template match="bean/method-arg">
  <constructor-arg>
    <xsl:copy-of select="@value|@ref|@class"/>
    <xsl:apply-templates select="bean|ref"/>
    <xsl:if test="not(@type) or @type='bean'">
      <xsl:if test="not(@ref) and not(bean) and not(ref)">
	<null/>
      </xsl:if>
    </xsl:if>
  </constructor-arg>
</xsl:template>

<xsl:template match="ioc">
  <xsl:if test="not(method-arg)">
   <property>
     <xsl:attribute name="name">
       <xsl:value-of select="@method"/>
     </xsl:attribute>
     <null/>
   </property>
  </xsl:if>

  <xsl:if test="count(method-arg)=1">
    <property>
      <xsl:attribute name="name">
	<xsl:value-of select="@method"/>
      </xsl:attribute>
      <xsl:copy-of select="method-arg/@value|method-arg/@ref"/>
      <xsl:apply-templates select="method-arg/bean|method-arg/ref"/>
      <xsl:if test="not(method-arg/@type) or method-arg/@type='bean'">
	<xsl:if test="not(method-arg/@ref) and not(method-arg/bean) and not(method-arg/ref)">
	</xsl:if>
      </xsl:if>
    </property>
  </xsl:if>

  <xsl:if test="count(method-arg)>1">
    <xsl:for-each select="method-arg">
      <property>
	<xsl:attribute name="name">
	  <xsl:value-of select="concat(../@method, '_arg_', position() - 1)"/>
	</xsl:attribute>
	<xsl:copy-of select="@value|@ref"/>
	<xsl:apply-templates select="bean|ref"/>
	<xsl:if test="not(@type) or @type='bean'">
	  <xsl:if test="not(@ref) and not(bean) and not(ref)">
	    <null/>
	  </xsl:if>
	</xsl:if>
      </property>
    </xsl:for-each>
  </xsl:if>
</xsl:template>

<xsl:template match="ref">
  <ref>
    <xsl:attribute name="local">
      <xsl:value-of select="@bean"/>
    </xsl:attribute>
  </ref>
</xsl:template>

</xsl:transform>
