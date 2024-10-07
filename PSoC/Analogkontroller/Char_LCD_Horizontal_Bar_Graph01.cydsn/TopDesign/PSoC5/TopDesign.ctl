-- =============================================================================
-- The following directives assign pins to the locations specific for the
-- CY8CKIT-050 and CY8CKIT-001 kits.
-- =============================================================================

-- === LCD pins ===
attribute port_location of \LCD:LCDPort(0)\ : label is "PORT(2,0)";
attribute port_location of \LCD:LCDPort(1)\ : label is "PORT(2,1)";
attribute port_location of \LCD:LCDPort(2)\ : label is "PORT(2,2)";
attribute port_location of \LCD:LCDPort(3)\ : label is "PORT(2,3)";
attribute port_location of \LCD:LCDPort(4)\ : label is "PORT(2,4)";
attribute port_location of \LCD:LCDPort(5)\ : label is "PORT(2,5)";
attribute port_location of \LCD:LCDPort(6)\ : label is "PORT(2,6)";