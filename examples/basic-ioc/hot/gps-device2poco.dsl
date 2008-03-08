<?xml version="1.0" encoding="iso-8859-1"?>

<!--
	A user defined gps model schema to poco core schema transformation
	stylesheet expressed in poco-dsl templates.

	Copyright 2007 by Pocomatic Software LLC, All Rights Reserved.

	Author: Ke Jin <kejin@pocomatic.com>
-->

<?xml-transform type="text/xsl" 
                href="http://www.pocomatic.com/poco-dsl2xsl.xsl"?>

<poco-dsl-transform>

<!-- gps-device is mapped to poco-application-context -->
<dsl-template match="/gps-device">
  <poco-application-context>
    <dsl-apply-templates/>
    <load library="./reflx.$dll"/>
  </poco-application-context>
</dsl-template>

<dsl-template match="tick-generator">
  <dsl-apply-templates select="@use" mode="load"/>
  <bean class="@use" destroy-method="delete" lazy-init="false">
    <method-arg type="short" value="@count"/>
    <method-arg type="short" value="@interval"/>
    <ioc method="subscribe">
      <method-arg ref="gps-locator"/>
    </ioc>
    <ioc method="start"/>
  </bean>
</dsl-template>

<dsl-template match="gps-locator">
  <dsl-apply-templates select="@use" mode="load"/>
  <bean class="@use" id="gps-locator" destroy-method="delete">
    <dsl-apply-templates select="../navigation-display" mode="subscribe"/>
  </bean>
</dsl-template>

<dsl-template match="navigation-display">
  <dsl-apply-templates select="@use" mode="load"/>
  <bean class="@use" id="#concat('nav-display-', generate-id())" 
        destroy-method="delete">
    <method-arg ref="gps-locator"/>
  </bean>
</dsl-template>

<dsl-template match="@use" mode="load">
  <load library="#concat('./', ., '.$dll')"/>
</dsl-template>

<dsl-template match="navigation-display" mode="subscribe">
  <ioc method="subscribe">
    <method-arg ref="#concat('nav-display-', generate-id())"/>
  </ioc>
</dsl-template>

</poco-dsl-transform>
