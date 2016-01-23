!
! Makefile for snmpget, snmpgetnext, snmpset...
!

USE_DECC = TRUE

!++
! Sort out any passed configuration options here. Produce the symbols
! C_QUAL, MAC_QUAL, LINK_QUAL as required. If you have any passed configuration
! options, delete the commented example which implements DEBUG and TRACE
! configuration options, and CHANGE THIS COMMENT!
!==

.IFDEF DEBUG			! Want to build system with DEBUG
C_QUAL = /DEBUG/NOOPTIMIZE
LINK_QUAL = /DEBUG
.ELSE
C_QUAL = /DEBUG=TRACEBACK
.IFDEF TRACE			! If not, may specify TRACE
LINK_QUAL = /TRACE
.ELSE
LINK_QUAL = /NOTRACE
.ENDIF
.ENDIF

.SUFFIXES
.SUFFIXES  .exe .obj .c .c$ .c~ .c$~ .mar .msg

.C.EXE :
	$(CC) $(CFLAGS) $(MMS$SOURCE)
	$(LINK) $(MMS$TARGET_NAME) $(MMS$SOURCE_NAME),SNMPLIB/LIBR

TARG =	SNMPGET.EXE SNMPGETNEXT.EXE SNMPSET.EXE SNMPTRANSLATE.EXE SNMPWALK.EXE

* : $(TARG)
	! finished

!++
! Sort out generation of analysis and listing files by compilers, and
! generation of map files by linker
!==

.IFDEF MMS_ANA
TMP_ANA = /ANALYSIS_DATA=$(MMS$TARGET_NAME)
.ELSE
TMP_ANA = /NOANALYSIS_DATA
.ENDIF

.IFDEF MMS_DIA
TMP_DIA = /DIAG
.ELSE
TMP_DIA =
.ENDIF

.IFDEF MMS_LIS
TMP_LIS = /LIST=$(MMS$TARGET_NAME)
.ELSE
TMP_LIS = /NOLIST
.ENDIF

.IFDEF MMS_MAP
TMP_MAP = /MAP=$(MMS$TARGET_NAME)/FULL
.ELSE
TMP_MAP = /NOMAP
.ENDIF

.IFDEF COMMON_EXTERN
TMP_CCEXT = /EXTE=COMM
.ELSE
TMP_CCEXT = 
.ENDIF

.IFDEF C_DEFINES
TMP_DEFINES = /DEFINE=($(C_DEFINES))
.ELSE						
TMP_DEFINES = 
.ENDIF

.IFDEF USE_DECC
CFLAGS = /STAND=RELAXED/SHARE/NOMEMBER/PREFIX=ALL/OBJECT=$(MMS$TARGET_NAME)$(TMP_ANA)$(TMP_DIA)$(TMP_LIS)$(TMP_CCEXT)$(C_QUAL)$(TMP_DEFINES)
.ELSE
CFLAGS = /STAND=VAXC/SHARE/NOMEMBER/OBJECT=$(MMS$TARGET_NAME)$(TMP_ANA)$(TMP_DIA)$(TMP_LIS)$(TMP_CCEXT)$(C_QUAL)$(TMP_DEFINES)
.ENDIF



SNMPGET.EXE : -
		SNMPGET.C,-
		NET-SNMP-CONFIG.H,-
		SNMP_UTILITIES.H,-
		NET-SNMP-INCLUDES.H
SNMPGETNEXT.EXE : -
		SNMPGETNEXT.C,-
		NET-SNMP-CONFIG.H,-
		SNMP_UTILITIES.H,-
		NET-SNMP-INCLUDES.H
SNMPSET.EXE : -
		SNMPSET.C,-
		NET-SNMP-CONFIG.H,-
		NET-SNMP-INCLUDES.H
SNMPTRANSLATE.EXE : -
		SNMPTRANSLATE.C,-
		NET-SNMP-CONFIG.H,-
		SNMP_UTILITIES.H,-
		SNMP_CONFIG_API.H,-
		SNMP_OUTPUT_API.H,-
		SNMP_MIB_API.H
SNMPWALK.EXE : -
		SNMPWALK.C,-
		NET-SNMP-CONFIG.H,-
		NET-SNMP-INCLUDES.H
