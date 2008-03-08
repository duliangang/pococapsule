<xsl:transform version  = '1.0'
                xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
                                                                                
<!--

   Stylesheet to transform CORBA with DDS extension support to 
   CORBA application context.                                                                        
   Copyright Pocomatic LLC. 2006, All Rights Reserved.
   Author: Ke Jin <kejin@pocomatic.com>
                                                                                
-->

<xsl:output doctype-system =
       "http://www.pocomatic.com/corba-application-context.dtd"/>
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

<xsl:template match="/corba-dds-application">
  <xsl:processing-instruction name="xml-transform">
    type="text/xsl" href="http://www.pocomatic.com/corba2poco.xsl"
  </xsl:processing-instruction>

  <corba-application-context>
    <header>#include "ddshelper.h"</header>

    <xsl:copy-of select="@id"/>
    <xsl:apply-templates/>
  </corba-application-context>
</xsl:template>

<!-- add id to orb -->
<xsl:template match="orb">
  <orb>
    <xsl:if test="not(@id)">
      <xsl:attribute name="id">
        <xsl:value-of select="concat('pocomatic.ddsorb.', generate-id(.))"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:copy-of select="@*"/>
    <xsl:apply-templates/>
  </orb>
</xsl:template>

<!-- element dds-participant -->
<xsl:template match="orb/dds-participant">
  <xsl:variable name="my-id">
    <xsl:if test="@id">
      <xsl:value-of select="@id"/>
    </xsl:if>
    <xsl:if test="not(@id)">
      <xsl:value-of
        select="concat('poco-id:dds-participant-', generate-id(.))"/>
    </xsl:if>
  </xsl:variable>

  <xsl:variable name="my-factory-id">
      <xsl:value-of
        select="concat('poco-id:dds-participant-factory-', generate-id(.))"/>
  </xsl:variable>

  <bean class="*DDS::DomainParticipantFactory_ptr"
        factory-method="poco_dds_get_factory">
    <xsl:attribute name="id">
      <xsl:value-of select="$my-factory-id"/>
    </xsl:attribute>

    <method-arg>
      <xsl:attribute name="ref">
        <xsl:if test="../@id"><xsl:value-of select="../@id"/></xsl:if>
        <xsl:if test="not(../@id)">
          <xsl:value-of select="concat('pocomatic.ddsorb.', generate-id(..))"/>
        </xsl:if>
      </xsl:attribute>
    </method-arg>
  </bean>

  <bean class="*DDS::DomainParticipant_ptr"
        factory-method="poco_dds_create_participant"
        dup-method="DDS::DomainParticipant::_duplicate"
        destroy-method="POCO_DDS_DELETE_PARTICIPANT">
    <xsl:attribute name="destroy-bean">
      <xsl:value-of select="$my-factory-id"/>
    </xsl:attribute>
    <xsl:attribute name="id">
      <xsl:value-of select="$my-id"/>
    </xsl:attribute>
    <xsl:if test="dds-listener|@listener">
      <xsl:attribute name="lazy-init">false</xsl:attribute>
    </xsl:if>

    <method-arg>
      <xsl:attribute name="ref">
        <xsl:value-of select="$my-factory-id"/>
      </xsl:attribute>
    </method-arg> 
    <method-arg type="long">
      <xsl:attribute name="value">
        <xsl:value-of select="@domain"/>
      </xsl:attribute>
    </method-arg>

    <method-arg class="DDS::DomainParticipantQos">
      <xsl:apply-templates select="dds-participant-qos"/>
    </method-arg>
    <xsl:call-template name="listener-arg"/>

    <xsl:copy-of select="ioc"/>
  </bean>

  <bean class="*DDS::Subscriber_ptr"
        factory-method="poco_dds_create_subscriber"
        dup-method="DDS::Subscriber::_duplicate"
        destroy-method="POCO_DDS_DELETE_SUBSCRIBER">
    <xsl:attribute name="id">
      <xsl:value-of select="concat($my-id, '-default-subscriber')"/>
    </xsl:attribute>
    <xsl:attribute name="destroy-bean">
      <xsl:value-of select="$my-id"/>
    </xsl:attribute>

    <method-arg>
      <xsl:attribute name="ref">
        <xsl:value-of select="$my-id"/>
      </xsl:attribute>
    </method-arg>
    <method-arg class="DDS::SubscriberQos"/>
    <method-arg class="*DDS::SubscriberListener_ptr"/>
  </bean>

  <bean class="*DDS::Publisher_ptr"
        factory-method="poco_dds_create_publisher"
        dup-method="DDS::Publisher::_duplicate"
        destroy-method="POCO_DDS_DELETE_PUBLISHER">
    <xsl:attribute name="id">
      <xsl:value-of select="concat($my-id, '-default-publisher')"/>
    </xsl:attribute>
    <xsl:attribute name="destroy-bean">
      <xsl:value-of select="$my-id"/>
    </xsl:attribute>

    <method-arg>
      <xsl:attribute name="ref">
        <xsl:value-of select="$my-id"/>
      </xsl:attribute>
    </method-arg>
    <method-arg class="DDS::PublisherQos"/>
    <method-arg class="*DDS::PublisherListener_ptr"/>
  </bean>

  <xsl:apply-templates 
    select="dds-topic|dds-data-reader|dds-data-writer|dds-subscriber|dds-publisher"/>

  <xsl:copy-of select="bean"/>
</xsl:template>

<!-- element of dds-topic -->
<xsl:template match="dds-topic">
  <xsl:variable name="participant-bean-id">
    <xsl:if test="../@id">
      <xsl:value-of select="../@id"/>
    </xsl:if>
    <xsl:if test="not(../@id)">
      <xsl:value-of select="concat('poco-id:dds-participant-', generate-id(..))"/>
    </xsl:if>
  </xsl:variable>

  <xsl:variable name="topic-bean-id">
    <xsl:if test="@id">
      <xsl:value-of select="@id"/>
    </xsl:if>
    <xsl:if test="not(@id)">
      <xsl:value-of select="concat('poco-id:dds-topic-', generate-id(.))"/>
    </xsl:if>
  </xsl:variable>

  <xsl:variable name="typesupport-bean-id">
    <xsl:value-of select="concat($topic-bean-id, '-typesupport')"/>
  </xsl:variable>
 
  <!-- the typesupport -->
  <xsl:comment>transformed from dds-topic</xsl:comment>
  <bean>
    <xsl:attribute name="class">
      <xsl:value-of select="concat(@type, 'TypeSupportImpl')"/>
    </xsl:attribute>
    <xsl:attribute name="id">
      <xsl:value-of select="$typesupport-bean-id"/>
    </xsl:attribute>
    <xsl:attribute name="key">
      <xsl:value-of select="concat($participant-bean-id, '-type-', @type, 'TypeSupportImpl')"/>
    </xsl:attribute>
    <ioc method="register_type">
      <method-arg>
        <xsl:attribute name="ref">
          <xsl:value-of select="$participant-bean-id"/>
        </xsl:attribute>
      </method-arg>
      <method-arg type="string">
        <xsl:attribute name="value">
          <xsl:value-of select="@type"/>
        </xsl:attribute>
      </method-arg>
    </ioc>
  </bean>

  <xsl:comment>transformed from dds-topic</xsl:comment>
  <bean class="*DDS::Topic_ptr"
        factory-method="poco_dds_create_topic"
        dup-method="DDS::Topic::_duplicate"
        destroy-method="POCO_DDS_DELETE_TOPIC">
    <xsl:attribute name="id">
      <xsl:value-of select="$topic-bean-id"/>
    </xsl:attribute>
    <xsl:attribute name="depends-on">
      <xsl:value-of select="$typesupport-bean-id"/>
    </xsl:attribute>
    <xsl:attribute name="destroy-bean">
      <xsl:value-of select="$participant-bean-id"/>
    </xsl:attribute>
    <xsl:if test="dds-listener|@listener">
      <xsl:attribute name="lazy-init">false</xsl:attribute>
    </xsl:if>

    <method-arg>
      <xsl:attribute name="ref">
        <xsl:value-of select="$participant-bean-id"/>
      </xsl:attribute>
    </method-arg>
    <method-arg type="string">
      <xsl:attribute name="value"><xsl:value-of select="@name"/></xsl:attribute>
    </method-arg> 
    <method-arg>
      <bean class="*char*" 
            factory-method="get_type_name"
            destroy-method="CORBA::string_free">
        <xsl:attribute name="factory-bean">
          <xsl:value-of select="$typesupport-bean-id"/>
        </xsl:attribute>
      </bean>
    </method-arg>
    <method-arg class="DDS::TopicQos">
      <xsl:apply-templates select="dds-topic-qos"/>
    </method-arg>
    <method-arg>
      <xsl:choose>
      <xsl:when test="@listener">
        <xsl:attribute name="ref">
          <xsl:value-of select="@listener"/>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="dds-listener">
        <xsl:apply-templates select="dds-listener"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:attribute name="class">
          <xsl:value-of select="'*CORBA::Object_ptr'"/>
        </xsl:attribute>
      </xsl:otherwise>
      </xsl:choose>
    </method-arg>

    <xsl:copy-of select="ioc"/>
  </bean>
</xsl:template>

<xsl:template match="dds-subscriber">
  <xsl:variable name="participant-bean-id">
    <xsl:if test="../@id">
      <xsl:value-of select="../@id"/>
    </xsl:if>
    <xsl:if test="not(../@id)">
      <xsl:value-of
        select="concat('poco-id:dds-participant-', generate-id(..))"/>
    </xsl:if>
  </xsl:variable>

  <xsl:comment>transformed from dds-subscriber</xsl:comment>
  <bean class="*DDS::Subscriber_ptr"
        factory-method="poco_dds_create_subscriber"
        dup-method="DDS::Subscriber::_duplicate"
        destroy-method="POCO_DDS_DELETE_SUBSCRIBER">
    <xsl:attribute name="destroy-bean">
      <xsl:value-of select="$participant-bean-id"/>
    </xsl:attribute>
    <xsl:attribute name="id">
      <xsl:if test="@id">
        <xsl:value-of select="@id"/>
      </xsl:if>
      <xsl:if test="not(@id)">
        <xsl:value-of select="concat('poco-id:dds-subscriber-', generate-id(.))"/>
      </xsl:if>
    </xsl:attribute>
    <xsl:if test="dds-listener|@listener">
      <xsl:attribute name="lazy-init">false</xsl:attribute>
    </xsl:if>

    <method-arg>
      <xsl:attribute name="ref">
        <xsl:value-of select="$participant-bean-id"/>
      </xsl:attribute>
    </method-arg>
    <method-arg class="DDS::SubscriberQos">
      <xsl:apply-templates select="dds-subscriber-qos"/>
    </method-arg>
    <xsl:call-template name="listener-arg"/>

    <xsl:copy-of select="ioc"/>
  </bean>
  <xsl:apply-templates select="dds-data-reader"/>
</xsl:template>

<xsl:template match="dds-publisher">
  <xsl:variable name="participant-bean-id">
    <xsl:if test="../@id">
      <xsl:value-of select="../@id"/>
    </xsl:if>
    <xsl:if test="not(../@id)">
      <xsl:value-of
        select="concat('poco-id:dds-participant-', generate-id(..))"/>
    </xsl:if>
  </xsl:variable>

  <xsl:comment>transformed from dds-publisher</xsl:comment>
  <bean class="*DDS::Publisher_ptr"
        factory-method="poco_dds_create_publisher"
        dup-method="DDS::Publisher::_duplicate"
        destroy-method="POCO_DDS_DELETE_PUBLISHER">
    <xsl:attribute name="destroy-bean">
      <xsl:value-of select="$participant-bean-id"/>
    </xsl:attribute>
    <xsl:attribute name="id">
      <xsl:if test="@id">
        <xsl:value-of select="@id"/>
      </xsl:if>
      <xsl:if test="not(@id)">
        <xsl:value-of select="concat('poco-id:dds-publisher-', generate-id(.))"/>
      </xsl:if>
    </xsl:attribute>
    <xsl:if test="dds-listener|@listener">
      <xsl:attribute name="lazy-init">false</xsl:attribute>
    </xsl:if>

    <method-arg>
      <xsl:attribute name="ref">
        <xsl:value-of select="$participant-bean-id"/>
      </xsl:attribute>
    </method-arg>
    <method-arg class="DDS::PublisherQos">
      <xsl:apply-templates select="dds-publisher-qos"/>
    </method-arg>
    <xsl:call-template name="listener-arg"/>
 
    <xsl:copy-of select="ioc"/>
  </bean>
  <xsl:apply-templates select="dds-data-writer"/>
</xsl:template>

<xsl:template match="dds-data-reader">
  <xsl:variable name="subscriber-bean-id">
    <xsl:if test="name(..)='dds-participant'">
      <xsl:if test="../@id">
        <xsl:value-of select="concat(../@id, '-default-subscriber')"/>
      </xsl:if>
      <xsl:if test="not(../@id)">
        <xsl:value-of
          select="concat('poco-id:dds-participant-', generate-id(..), '-default-subscriber')"/>
      </xsl:if>
    </xsl:if> 
    <xsl:if test="name(..)='dds-subscriber'">
      <xsl:if test="../@id">
        <xsl:value-of select="../@id"/>
      </xsl:if>
      <xsl:if test="not(../@id)">
        <xsl:value-of
          select="concat('poco-id:dds-subscriber-', generate-id(..))"/>
      </xsl:if>
    </xsl:if>
  </xsl:variable>

  <xsl:comment>transformed from dds-data-reader</xsl:comment>
  <bean class="*DDS::DataReader_ptr"
        factory-method="poco_dds_create_datareader"
        dup-method="DDS::DataReader::_duplicate"
        destroy-method="POCO_DDS_DELETE_DATAREADER">
    <xsl:attribute name="destroy-bean">
      <xsl:value-of select="$subscriber-bean-id"/>
    </xsl:attribute>
    <xsl:attribute name="id">
      <xsl:if test="@id">
        <xsl:value-of select="@id"/>
      </xsl:if>
      <xsl:if test="not(@id)">
        <xsl:value-of select="concat('poco-id:dds-reader-', generate-id(..))"/>
      </xsl:if>
    </xsl:attribute>
    <xsl:if test="dds-listener|@listener">
      <xsl:attribute name="lazy-init">false</xsl:attribute>
    </xsl:if>

    <method-arg>
      <xsl:attribute name="ref">
        <xsl:value-of select="$subscriber-bean-id"/>
      </xsl:attribute>
    </method-arg>
    <method-arg>
      <xsl:attribute name="ref">
        <xsl:value-of select="@topic"/>
      </xsl:attribute>
    </method-arg>
    <method-arg class="DDS::DataReaderQos">
      <xsl:apply-templates select="dds-reader-qos"/>
    </method-arg>
    <xsl:call-template name="listener-arg"/>

    <xsl:copy-of select="ioc"/>
  </bean>
</xsl:template>

<xsl:template match="dds-data-writer">
  <xsl:variable name="publisher-bean-id">
    <xsl:if test="name(..)='dds-participant'">
      <xsl:if test="../@id">
        <xsl:value-of select="concat(../@id, '-default-publisher')"/>
      </xsl:if>
      <xsl:if test="not(../@id)">
        <xsl:value-of
          select="concat('poco-id:dds-participant-', generate-id(..), '-default-publisher')"/>
      </xsl:if>
    </xsl:if> 
    <xsl:if test="name(..)='dds-publisher'">
      <xsl:if test="../@id">
        <xsl:value-of select="../@id"/>
      </xsl:if>
      <xsl:if test="not(../@id)">
        <xsl:value-of
          select="concat('poco-id:dds-publisher-', generate-id(..))"/>
      </xsl:if>
    </xsl:if>
  </xsl:variable>

  <xsl:comment>transformed from dds-data-writer</xsl:comment>
  <bean class="*DDS::DataWriter_ptr"
        factory-method="poco_dds_create_datawriter"
        dup-method="DDS::DataWriter::_duplicate"
        destroy-method="POCO_DDS_DELETE_DATAWRITER">
    <xsl:attribute name="destroy-bean">
      <xsl:value-of select="$publisher-bean-id"/>
    </xsl:attribute>
    <xsl:attribute name="id">
      <xsl:if test="@id">
        <xsl:value-of select="@id"/>
      </xsl:if>
      <xsl:if test="not(@id)">
        <xsl:value-of select="concat('poco-id:dds-writer-', generate-id(..))"/>
      </xsl:if>
    </xsl:attribute>
    <xsl:if test="dds-listener|@listener">
      <xsl:attribute name="lazy-init">false</xsl:attribute>
    </xsl:if>

    <method-arg>
      <xsl:attribute name="ref">
        <xsl:value-of select="$publisher-bean-id"/>
      </xsl:attribute>
    </method-arg>
    <method-arg>
      <xsl:attribute name="ref">
        <xsl:value-of select="@topic"/>
      </xsl:attribute>
    </method-arg>
    <method-arg class="DDS::DataWriterQos">
      <xsl:apply-templates select="dds-writer-qos"/>
    </method-arg>
    <xsl:call-template name="listener-arg"/>

    <xsl:copy-of select="ioc"/>
  </bean>
</xsl:template>

<xsl:template name="listener-arg">
  <method-arg>
    <xsl:choose>
    <xsl:when test="@listener">
     <xsl:attribute name="ref">
        <xsl:value-of select="@listener"/>
      </xsl:attribute>
    </xsl:when>
    <xsl:when test="dds-listener">
      <xsl:apply-templates select="dds-listener"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:attribute name="class">
        <xsl:value-of select="'*CORBA::Object_ptr'"/>
      </xsl:attribute>
    </xsl:otherwise>
    </xsl:choose>
  </method-arg>
</xsl:template>

<xsl:template match="dds-listener">
  <xsl:copy-of select="bean"/>
</xsl:template>

</xsl:transform>
