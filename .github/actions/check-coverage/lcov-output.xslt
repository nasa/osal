<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="xml" omit-xml-declaration="yes" encoding="UTF-8"/>

<!-- Identity Template -->
<xsl:template match="@* | node()">
  <xsl:copy>
    <xsl:apply-templates select="@* | node()"/>
  </xsl:copy>
</xsl:template>

<!-- Omit class attributes, width attributes, and img elements -->
<xsl:template match="@class" />
<xsl:template match="@width" />
<xsl:template match="@align" />
<xsl:template match="img" />

<!-- The "coverBar" will not render on github, so pull out the alt text on the image -->
<xsl:template match="td[@class='coverBar']">
  <td>
    <xsl:for-each select=".//img[1]">
      <xsl:if test="@width &lt; 95">X</xsl:if>
    </xsl:for-each>
  </td>
</xsl:template>

<!-- Convert td w/class="tableHead" to a th tag -->
<xsl:template match="td[@class='tableHead']">
  <th>
    <xsl:if test="count(@colspan) > 0">
      <xsl:attribute name="colspan"><xsl:value-of select="@colspan"/></xsl:attribute>
    </xsl:if>
    <xsl:for-each select="text()">
      <xsl:copy/>
    </xsl:for-each>
  </th>
</xsl:template>

<xsl:template match="span|center">
  <xsl:apply-templates select="@* | node()"/>
</xsl:template>

<!-- Rewrite links to be plain text -->
<xsl:template match="a">
  <xsl:for-each select="text()">
    <xsl:copy/>
  </xsl:for-each>
</xsl:template>

<xsl:template name="summary_row">
  <!-- Extract only cells 4-7 here (label, hit, total, coverage) -->
  <xsl:for-each select="td[position() &gt;= 4]">
    <xsl:copy>
      <xsl:apply-templates select="@* | node()" />
    </xsl:copy>
  </xsl:for-each>
  <xsl:text>
  </xsl:text>
</xsl:template>

<xsl:template name="summary_table">
  <table>
    <!-- Extract only rows 1-4 here (header, lines, functions, branches) -->
    <xsl:for-each select="tr[position() &lt;= 4]">
      <xsl:copy>
        <xsl:call-template name="summary_row" />
      </xsl:copy>
    </xsl:for-each>
  </table>
  <xsl:text>
  </xsl:text>
</xsl:template>

<xsl:template name="detail_table">
  <table>
    <!-- First row appears to be always blank/spacer, real content starts at 2 -->
    <xsl:for-each select="tr[position() &gt;= 2]">
      <xsl:copy>
        <xsl:apply-templates select="@* | node()"/>
      </xsl:copy>
    </xsl:for-each>
  </table>
  <xsl:text>
  </xsl:text>
</xsl:template>

<!-- The LCOV output uses tables for white-spacing purposes as well as actual tabular data -->
<xsl:template match="/">
  <h2>LCOV Report</h2>
  <xsl:text>
  </xsl:text>
  <!-- The first table is the summary, but we really want the embedded table within the table at row 3 -->
  <xsl:for-each select="/html/body/table[1]/tr[3]/td/table">
    <xsl:call-template name="summary_table" />
  </xsl:for-each>

  <!-- The centered table is the file-by-file details -->
  <xsl:for-each select="/html/body/center/table[1]">
    <xsl:call-template name="detail_table" />
  </xsl:for-each>

</xsl:template>


</xsl:stylesheet>
