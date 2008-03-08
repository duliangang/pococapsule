<xsl:transform version  = '1.0'
                xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
                                                                                
<!--
                                                                                
   corba-app-context to poco-app-context transform template.

   Copyright Pocomatic LLC. 2006, All Rights Reserved.
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

<xsl:template match="/corba-application-context">
  <poco-application-context>
    <xsl:comment> transformed context </xsl:comment>
    <header>#include "corbahelper.h"</header>
    <xsl:apply-templates/>
  </poco-application-context>
</xsl:template>

<xsl:template match="orb">
  <xsl:variable name="orb-id">
    <xsl:if test="@id">
      <xsl:value-of select="@id"/>
    </xsl:if>
    <xsl:if test="not(@id)">
      <xsl:value-of select="concat('pocomatic.orb.', generate-id(.))"/>
    </xsl:if>
  </xsl:variable>

  <!-- the ORB bean -->
  <xsl:comment> transformed from the &lt;orb&gt; element </xsl:comment>
  <bean class="*CORBA::ORB_ptr"
        factory-method="POCO_CORBAHelper::ORB_init"
        singleton="true"
        lazy-init="false">
    <xsl:copy-of select="@label"/>

    <xsl:attribute name="id">
        <xsl:value-of select="$orb-id"/>
    </xsl:attribute>

    <method-arg type="array">
      <array type="string" env-var="pococapsule.init.argv">
        <xsl:for-each select="args/arg">
          <item>
            <xsl:attribute name="value">
              <xsl:value-of select="@value"/>
            </xsl:attribute>
          </item>
        </xsl:for-each>
      </array>
    </method-arg>
  </bean>

  <!-- the implicit and lazy-init'ed root POA bean -->
  <xsl:comment> the implicit root POA from the &lt;orb&gt; element </xsl:comment>
  <bean class="*PortableServer::POA_ptr"
        factory-method="PortableServer::POA::_narrow"
        singleton="true"
        lazy-init="true"
        dup-method="PortableServer::POA::_duplicate">
    <xsl:attribute name="id">
      <xsl:value-of select="concat('pocomatic.poa.', generate-id(.))"/>
    </xsl:attribute>

    <method-arg class="*CORBA::Object_ptr">
      <bean class="*CORBA::Object_ptr"
            factory-method="resolve_initial_references"
            singleton="false"
            lazy-init="true"
            destroy-method="CORBA::release">
         <xsl:attribute name="factory-bean">
           <xsl:value-of select="$orb-id"/>
         </xsl:attribute>
         <method-arg type="string">
           <xsl:attribute name="value">
             <xsl:value-of select="'RootPOA'"/>
           </xsl:attribute>
         </method-arg>
      </bean>
    </method-arg>

    <ioc method="the_POAManager()->activate"/>
  </bean>

  <!-- the implicit ORB dispatcher -->
  <xsl:comment> the implicit ORB dispatcher </xsl:comment>
  <bean class="void" factory-method="POCO_CORBAHelper::run">
    <xsl:if test="@id">   
      <xsl:attribute name="id">
        <xsl:value-of select="concat('pocomatic.dispatcher:', @id)"/>
      </xsl:attribute>
      <xsl:attribute name="lazy-init">true</xsl:attribute>
      <xsl:attribute name="singleton">true</xsl:attribute>
      <method-arg class="*CORBA::ORB_ptr">
        <xsl:attribute name="ref">
          <xsl:value-of select="@id"/>
        </xsl:attribute>
      </method-arg>
    </xsl:if>
    <xsl:if test="not(@id)">
      <xsl:attribute name="singleton">false</xsl:attribute>
      <method-arg class="*CORBA::ORB_ptr"/>
    </xsl:if>
  </bean>
 
  <!-- sub-nodes of <orb> -->
  <xsl:apply-templates 
    select="event-consumer-adapter|event-supplier-adapter|bean|object|poa"/> 
</xsl:template>

<xsl:template match="poa">
  <xsl:comment> transformed from a &lt;poa&gt; element </xsl:comment>
  <bean class="*PortableServer::POA_ptr"
        factory-method="POCO_CORBAHelper::create_POA" 
        dup-method="PortableServer::POA::_duplicate"
        lazy-init="true"
        singleton="true">
    <xsl:if test="not(@id)">
      <xsl:attribute name="id">
        <xsl:value-of select="concat('pocomatic.poa.', generate-id(.))"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:copy-of select="@label|@id|@destroy-method"/>

    <!-- first argument is the parent POA -->
    <method-arg class="*PortableServer::POA_ptr">
      <xsl:choose>
      <xsl:when test="name(..)='orb'">
        <xsl:attribute name="ref">
          <xsl:value-of select="concat('pocomatic.poa.', generate-id(..))"/>
        </xsl:attribute>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
        <xsl:when test="../@id">
          <xsl:attribute name="ref">
            <xsl:value-of select="../@id"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="factory-bean">
            <xsl:value-of select="concat('pocomatic.poa.', generate-id(..))"/>
          </xsl:attribute>
        </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
      </xsl:choose>
    </method-arg>

    <!-- second argument is the poa name -->
    <method-arg type="string">
      <xsl:if test="@name">
        <xsl:attribute name="value">
          <xsl:value-of select="@name"/>
        </xsl:attribute>
      </xsl:if>
    </method-arg>

    <!-- third argument is the POA Manager -->
    <method-arg class="*PortableServer::POAManager_ptr">
      <xsl:if test="@poa-manager">
        <ref>
          <xsl:attribute name="bean">
            <xsl:value-of select="poa-manager"/>
          </xsl:attribute>
        </ref>
      </xsl:if>
    </method-arg>
       
    <!-- forth argument is the policy array -->
    <method-arg type="array">
      <array type="namevalue"> 
        <xsl:apply-templates select="policies"/>
      </array>
    </method-arg>

    <xsl:apply-templates select="on-error|ioc|property"/>
  </bean>

  <!-- objects and child POAs after it -->
  <xsl:apply-templates select="object|poa"/>
</xsl:template>

<xsl:template match="policies|props">
  <xsl:apply-templates select="policy|prop"/>
</xsl:template>

<xsl:template match="policy|prop">
  <item>
    <xsl:copy-of select="@name|@value"/>
  </item>
</xsl:template>

<xsl:template match="object">
  <xsl:comment> transformed from an &lt;object&gt; element </xsl:comment>
  <xsl:variable name="id">
    <xsl:if test="@id">
      <xsl:value-of select="@id"/>
    </xsl:if>
    <xsl:if test="not(@id)">
      <xsl:value-of select="concat('pocomatic.object.', generate-id(.))"/>
    </xsl:if>
  </xsl:variable>
 
  <bean class="*CORBA::Object_ptr"
        factory-method="POCO_CORBAHelper::export_object"
        dup-method="CORBA::Object::_duplicate"
	lazy-init="false" 
        singleton="true">
    <xsl:copy-of select="@label|@destroy-method"/>

    <xsl:if test="@id|@bind-name">
      <xsl:attribute name="id"><xsl:value-of select="$id"/></xsl:attribute>
    </xsl:if>

    <!-- first argument is the POA -->
    <method-arg class="*PortableServer::POA_ptr">
    <xsl:choose>
    <xsl:when test="name(..)='orb'">
      <xsl:attribute name="ref">
         <xsl:value-of select="concat('pocomatic.poa.', generate-id(..))"/>
      </xsl:attribute>
    </xsl:when>
    <xsl:otherwise>
      <xsl:choose>
      <xsl:when test="../@id">
      <xsl:attribute name="ref">
        <xsl:value-of select="../@id"/>
      </xsl:attribute>
      </xsl:when>
      <xsl:otherwise>
      <xsl:attribute name="ref">
        <xsl:value-of select="concat('pocomatic.poa.', generate-id(..))"/>
      </xsl:attribute>
      </xsl:otherwise>
      </xsl:choose>
    </xsl:otherwise>
    </xsl:choose>
    </method-arg>

    <!-- the second argument is oid -->
    <method-arg type="string">
      <xsl:if test="@oid">
        <xsl:attribute name="value">
          <xsl:value-of select="@oid"/>
        </xsl:attribute>
      </xsl:if>
    </method-arg>

    <!-- the third and fourth arguments -->
    <xsl:call-template name="servant"/>
    
    <!-- the last argument is uri -->
    <method-arg type="string">
      <xsl:if test="@uri">
        <xsl:attribute name="value">
          <xsl:value-of select="@uri"/>
        </xsl:attribute>
      </xsl:if>
    </method-arg>
    <xsl:copy-of select="on-error|ioc"/>
  </bean>

  <xsl:variable name="orb-id">
    <xsl:if test="/corba-application-context/orb/@id">
      <xsl:value-of select="/corba-application-context/orb/@id"/>
    </xsl:if>
    <xsl:if test="not(/corba-application-context/orb/@id)">
      <xsl:value-of select="concat('pocomatic.orb.', generate-id(/corba-application-context/orb))"/>
    </xsl:if>
  </xsl:variable>

  <xsl:if test="@bind-name">
    <bean class="void"
          factory-method="POCO_CORBAHelper::bind_to_ins"
          singleton="true"
          lazy-init="false">
       <method-arg class="*CORBA::Object_ptr">
         <bean class="*CORBA::Object_ptr"
               factory-method="resolve_initial_references"
               destroy-method="CORBA::release"
               singleton="false">
           <xsl:attribute name="factory-bean"><xsl:value-of select="$orb-id"/></xsl:attribute>
           <method-arg type="string" value="NameService"/>
         </bean>
       </method-arg>
       <method-arg type="string">
         <xsl:attribute name="value"><xsl:value-of select="@bind-name"/></xsl:attribute>
       </method-arg>
       <method-arg class="*CORBA::Object_ptr">
         <xsl:attribute name="ref"><xsl:value-of select="$id"/></xsl:attribute>
       </method-arg>
    </bean>
  </xsl:if>
</xsl:template>

<xsl:template name="servant">
  <xsl:if test="@impl-ref|bean|ref">
    <method-arg type="string">
      <xsl:if test="@repid">
        <xsl:attribute name="value">
          <xsl:value-of select="@repid"/>
        </xsl:attribute>
      </xsl:if>
    </method-arg>

    <method-arg>
      <xsl:if test="@impl-type='native'">
        <xsl:if test="not(@interface)">
          <transform-error message="Error at an &lt;object&gt; element: a impl-type=&quot;native&quot; should specify the &quot;interface&quot; attribute!"/>
        </xsl:if>
        <xsl:variable name='native-class'>
          <xsl:if test="@impl-ref">
            <xsl:value-of select="id(@impl-ref)/@class"/>
          </xsl:if>
          <xsl:if test="ref">
            <xsl:value-of select="id(ref/@bean)/@class"/>
          </xsl:if>
          <xsl:if test="bean">
            <xsl:value-of select="bean/@class"/>
          </xsl:if>
        </xsl:variable>
        <bean>
          <xsl:attribute name="class">
            <xsl:value-of select="concat('POA_', @interface, '_tie', '{', $native-class, '}')"/>
          </xsl:attribute>
          <method-arg>
            <xsl:if test="@impl-ref">
              <xsl:attribute name="ref">
                <xsl:value-of select="@impl-ref"/>
              </xsl:attribute>
            </xsl:if>
            <xsl:apply-templates select="bean|ref"/>
          </method-arg>
        </bean>
      </xsl:if>

      <xsl:if test="not(@impl-type='native')">
        <xsl:if test="@impl-ref">
          <xsl:attribute name="ref">
            <xsl:value-of select="@impl-ref"/>
          </xsl:attribute>
        </xsl:if>
      <xsl:copy-of select="bean|ref"/>
      </xsl:if>
    </method-arg>
  </xsl:if>

  <xsl:if test="not(@impl-ref|bean|ref)">
    <method-arg type="array">
      <array class="*PortableServer::Servant">
        <xsl:apply-templates select="servants"/>
      </array>
    </method-arg>
  </xsl:if>
</xsl:template>
    
<xsl:template match="servants">
  <xsl:for-each select="bean|ref">
    <item><xsl:copy-of select="."/></item>
  </xsl:for-each>
</xsl:template>

<xsl:template match="admin|channel|service">
   <xsl:if test="@ref">
     <xsl:copy-of select="@ref"/>
   </xsl:if>

   <xsl:if test="@ior">
     <bean class="*CORBA::Object_ptr"
           factory-method="string_to_object"
           destroy-method="CORBA::release"
           singleton="false">
        <xsl:choose>
        <xsl:when test="../../@id">
          <xsl:attribute name="factory-bean">
            <xsl:value-of select="../../@id"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="factory-bean">
             <xsl:value-of 
               select="concat('pocomatic.orb.', generate-id(../..))"/>
             </xsl:attribute>
        </xsl:otherwise>
        </xsl:choose>

        <method-arg type="string">
          <xsl:attribute name="value">
            <xsl:value-of select="@ior"/>
          </xsl:attribute>
        </method-arg>
     </bean>
   </xsl:if>

   <xsl:if test="not(@ior)">
     <xsl:copy-of select="bean|ref"/>
   </xsl:if>
</xsl:template>

<xsl:template name="default-service">
  <xsl:if test="not(admin|channel|service)">
    <bean class="*CORBA::Object_ptr"
          factory-method="string_to_object"
          destroy-method="CORBA::release"
          singleton="false">
      <xsl:choose>
      <xsl:when test="../@id">
      <xsl:attribute name="factory-bean">
      <xsl:value-of select="../@id"/>
        </xsl:attribute>
      </xsl:when>
      <xsl:otherwise>
      <xsl:attribute name="factory-bean">
        <xsl:value-of
          select="concat('pocomatic.orb.', generate-id(..))"/>
        </xsl:attribute>
      </xsl:otherwise>
      </xsl:choose>

      <method-arg type="string" value="corbaloc:rir:/NotificationService"/>
    </bean>
  </xsl:if>
</xsl:template>

<!-- event-consumer-adapter -->
<xsl:template match="event-consumer-adapter|event-supplier-adapter">
  <xsl:comment> transformed from an &lt;<xsl:value-of select="name()"/>&gt; element </xsl:comment>
  <bean class="*CORBA::Object_ptr"
        factory-method="POCO_CORBAHelper::subscribe"
        dup-method="CORBA::Object::_duplicate"
        singleton="true">
    <xsl:copy-of select="@label|@id"/>

    <xsl:if test="name()='event-consumer-adapter'">
      <xsl:attribute name="factory-method">
        <xsl:value-of select="'POCO_CORBAHelper::subscribe'"/>
      </xsl:attribute>
      <xsl:if test="not(@disconnect='manual')">
        <xsl:attribute name="destroy-method">
          <xsl:value-of select="'POCO_CORBAHelper::unsubscribe'"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="@type='typed'">
        <xsl:if test="not(@topic)">
          <xsl:message>+++ Warning at an &lt;event-consumer-adaptor&gt;: a typed consumer adaptor should have a topic attribute!</xsl:message>
        </xsl:if>
      </xsl:if>
    </xsl:if>

    <xsl:if test="name()='event-supplier-adapter'">
      <xsl:attribute name="factory-method">
        <xsl:value-of select="'POCO_CORBAHelper::publish'"/>
      </xsl:attribute>
      <xsl:if test="not(@disconnect='manual')">
        <xsl:attribute name="destroy-method">
          <xsl:value-of select="'POCO_CORBAHelper::unpublish'"/>
        </xsl:attribute>
      </xsl:if>
    </xsl:if>

    <xsl:choose>
    <xsl:when test="@lazy-init">
      <xsl:copy-of select="@lazy-init"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:attribute name="lazy-init">false</xsl:attribute>
    </xsl:otherwise>
    </xsl:choose>

    <!-- the first argument is the ORB -->
    <method-arg class="*CORBA::ORB_ptr">
      <xsl:choose>
      <xsl:when test="../@id">
          <xsl:attribute name="ref">
            <xsl:value-of select="../@id"/>
          </xsl:attribute>
      </xsl:when>
      <xsl:otherwise>
        <xsl:attribute name="ref">
           <xsl:value-of select="concat('pocomatic.orb.', generate-id(..))"/>
        </xsl:attribute>
      </xsl:otherwise>
      </xsl:choose>
    </method-arg>

    <!-- the second argument is notification service -->
    <method-arg class="*CORBA::Object_ptr">
      <xsl:apply-templates select="service"/>
      <xsl:call-template name="default-service"/>
    </method-arg>

    <!-- the third argument is the channel -->
    <method-arg class="*CORBA::Object_ptr">
      <xsl:apply-templates select="channel"/>
    </method-arg>

    <!-- the fourth argument is the admin -->
    <method-arg class="*CORBA::Object_ptr">
      <xsl:apply-templates select="admin"/>
    </method-arg>

    <!-- the fifth argument is the event type -->
    <method-arg type="string">
      <xsl:attribute name="value">
        <xsl:value-of select="@type"/>
      </xsl:attribute>
    </method-arg>
    
    <!-- the sixth argument is the topic --> 
    <method-arg type="string">
      <xsl:if test="@topic">
        <xsl:attribute name="value">
          <xsl:value-of select="@topic"/>
        </xsl:attribute>
      </xsl:if>
    </method-arg>

    <!-- the seventh argument is the props array -->
    <method-arg type="array">
      <array type="namevalue">
        <xsl:apply-templates select="props"/>
      </array>
    </method-arg>

    <!-- the eights argument is the consumer or supplier -->
    <method-arg>
      <xsl:if test="not(event-consumer|event-supplier)">
        <xsl:attribute name="class">*CORBA::Object_ptr</xsl:attribute>
      </xsl:if>
      <xsl:apply-templates select="event-consumer|event-supplier"/>
    </method-arg>

    <xsl:if test="event-supplier/@impl-type='native'">
      <xsl:variable name="supplier-class">
        <xsl:if test="event-supplier/@impl-ref">
          <xsl:value-of select="id(event-supplier/@impl-ref)/@class"/>
        </xsl:if>
        <xsl:if test="event-supplier/ref">
          <xsl:value-of select="id(event-supplier/ref/@bean)/@class"/>
        </xsl:if>
        <xsl:if test="event-supplier/bean">
          <xsl:value-of select="event-supplier/bean/@class"/>
        </xsl:if>
      </xsl:variable>

      <method-arg type="string">
        <xsl:attribute name="value">
          <xsl:value-of select="$supplier-class"/>
        </xsl:attribute>
      </method-arg>

      <bean abstract="true">
        <xsl:attribute name="class">
          <xsl:value-of select="$supplier-class"/>
        </xsl:attribute>
        <ioc> 
        <xsl:if test="@type='typed'">
          <xsl:attribute name="method">
            <xsl:value-of select="'connect_typed_push_consumer'"/>
          </xsl:attribute>
          <method-arg class="*CosTypedEventComm::TypedPushConsumer_ptr"/>
        </xsl:if>
        <xsl:if test="@type='untyped'">
          <xsl:attribute name="method">
            <xsl:value-of select="'connect_any_push_consumer'"/>
          </xsl:attribute>
          <method-arg class="*CosEventComm::PushConsumer_ptr"/>
        </xsl:if>
        <xsl:if test="@type='structured'">
          <xsl:attribute name="method">
            <xsl:value-of select="'connect_structured_push_consumer'"/>
          </xsl:attribute>
          <method-arg class="*CosNotifyComm::StructuredPushConsumer_ptr"/>
        </xsl:if>
        <xsl:if test="@type='sequence'">
          <xsl:attribute name="method">
            <xsl:value-of select="'connect_sequence_push_consumer'"/>
          </xsl:attribute>
          <method-arg class="*CosNotifyComm::SequencePushConsumer_ptr"/>
        </xsl:if>
        </ioc>
      </bean>
    </xsl:if>

    <xsl:if test="event-consumer/@impl-type='native' and not(@type='typed')">
      <xsl:variable name="consumer-class">
        <xsl:if test="event-consumer/@impl-ref">
          <xsl:value-of select="id(event-consumer/@impl-ref)/@class"/>
        </xsl:if>
        <xsl:if test="event-consumer/ref">
          <xsl:value-of select="id(event-consumer/ref/@bean)/@class"/>
        </xsl:if>
        <xsl:if test="event-consumer/bean">
          <xsl:value-of select="event-consumer/bean/@class"/>
        </xsl:if>
      </xsl:variable>

      <method-arg type="string">
        <xsl:attribute name="value">
          <xsl:value-of select="$consumer-class"/>
        </xsl:attribute>
      </method-arg>

      <bean abstract="true">
        <xsl:attribute name="class">
          <xsl:value-of select="$consumer-class"/>
        </xsl:attribute>
        <ioc>
        <xsl:if test="@type='untyped'">
          <xsl:attribute name="method">
            <xsl:value-of select="'push'"/>
          </xsl:attribute>
          <method-arg class="CORBA::Any" pass="deref"/>
        </xsl:if>
        <xsl:if test="@type='structured'">
          <xsl:attribute name="method">
            <xsl:value-of select="'push_structured_event'"/>
          </xsl:attribute>
          <method-arg class="CosNotification::StructuredEvent" pass="deref"/>
        </xsl:if>
        <xsl:if test="@type='sequence'">
          <xsl:attribute name="method">
            <xsl:value-of select="'push_structured_events'"/>
          </xsl:attribute>
          <method-arg class="CosNotification::EventBatch" pass="deref"/>
        </xsl:if>
        </ioc>
      </bean>
    </xsl:if>

  </bean>
</xsl:template>

<xsl:template match="event-consumer|event-supplier">
  <xsl:choose>
  <xsl:when test="name()='event-consumer' and ../@type='typed' and @impl-type='native'">
    <xsl:if test="not(@interface)">
        <transform-error message="Error at an &lt;event-consumer&gt; element: a typed consumer of impl-type=&quot;native&quot; should specify the &quot;interface&quot; attribute!"/>
    </xsl:if>

    <xsl:variable name='native-class'>
      <xsl:if test="@impl-ref">
        <xsl:value-of select="id(@impl-ref)/@class"/>
      </xsl:if>
      <xsl:if test="ref">
        <xsl:value-of select="id(ref/@bean)/@class"/>
      </xsl:if>
      <xsl:if test="bean">
        <xsl:value-of select="bean/@class"/>
      </xsl:if>
    </xsl:variable>

    <bean>
      <xsl:attribute name="class">
        <xsl:value-of select="concat('POA_', @interface, '_tie', '{', $native-class, '}')"/>
      </xsl:attribute>
      <method-arg>
        <xsl:if test="@impl-ref">
          <xsl:attribute name="ref">
            <xsl:value-of select="@impl-ref"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:apply-templates select="bean|ref"/>
      </method-arg>
    </bean>
  </xsl:when>
  <xsl:otherwise>
    <xsl:if test="@impl-type='object'">
      <xsl:attribute name="class">*CORBA::Object_ptr</xsl:attribute>
    </xsl:if>
    <xsl:if test="@impl-type='servant'">
      <xsl:attribute name="class">*PortableServer::Servant</xsl:attribute>
    </xsl:if>
    <xsl:if test="@impl-type='native'">
      <xsl:attribute name="class">*void*</xsl:attribute>
    </xsl:if>
    <xsl:if test="@impl-ref">
      <xsl:attribute name="ref">
        <xsl:value-of select="@impl-ref"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:apply-templates select="bean|ref"/>
    </xsl:otherwise>
  </xsl:choose> 
</xsl:template>

<xsl:template match="property">
  <xsl:comment> transformed from &lt;property&gt; element </xsl:comment>
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

</xsl:transform>
