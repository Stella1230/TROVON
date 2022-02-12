static u8 acpi_ex_interpreter_trace_enabled(char *name)
{
if (!(acpi_gbl_trace_flags & ACPI_TRACE_ENABLED)) {
return (FALSE);
}
if (acpi_gbl_trace_method_object) {
return (TRUE);
}
if (name &&
(acpi_gbl_trace_method_name &&
strcmp(acpi_gbl_trace_method_name, name))) {
return (FALSE);
}
if ((acpi_gbl_trace_flags & ACPI_TRACE_ONESHOT) &&
!acpi_gbl_trace_method_name) {
return (FALSE);
}
return (TRUE);
}
static const char *acpi_ex_get_trace_event_name(acpi_trace_event_type type)
{
switch (type) {
case ACPI_TRACE_AML_METHOD:
return "Method";
case ACPI_TRACE_AML_OPCODE:
return "Opcode";
case ACPI_TRACE_AML_REGION:
return "Region";
default:
return "";
}
}
void
acpi_ex_trace_point(acpi_trace_event_type type,
u8 begin, u8 *aml, char *pathname)
{
ACPI_FUNCTION_NAME(ex_trace_point);
if (pathname) {
ACPI_DEBUG_PRINT((ACPI_DB_TRACE_POINT,
"%s %s [0x%p:%s] execution.\n",
acpi_ex_get_trace_event_name(type),
begin ? "Begin" : "End", aml, pathname));
} else {
ACPI_DEBUG_PRINT((ACPI_DB_TRACE_POINT,
"%s %s [0x%p] execution.\n",
acpi_ex_get_trace_event_name(type),
begin ? "Begin" : "End", aml));
}
}
void
acpi_ex_start_trace_method(struct acpi_namespace_node *method_node,
union acpi_operand_object *obj_desc,
struct acpi_walk_state *walk_state)
{
acpi_status status;
char *pathname = NULL;
u8 enabled = FALSE;
ACPI_FUNCTION_NAME(ex_start_trace_method);
if (method_node) {
pathname = acpi_ns_get_normalized_pathname(method_node, TRUE);
}
status = acpi_ut_acquire_mutex(ACPI_MTX_NAMESPACE);
if (ACPI_FAILURE(status)) {
goto exit;
}
enabled = acpi_ex_interpreter_trace_enabled(pathname);
if (enabled && !acpi_gbl_trace_method_object) {
acpi_gbl_trace_method_object = obj_desc;
acpi_gbl_original_dbg_level = acpi_dbg_level;
acpi_gbl_original_dbg_layer = acpi_dbg_layer;
acpi_dbg_level = ACPI_TRACE_LEVEL_ALL;
acpi_dbg_layer = ACPI_TRACE_LAYER_ALL;
if (acpi_gbl_trace_dbg_level) {
acpi_dbg_level = acpi_gbl_trace_dbg_level;
}
if (acpi_gbl_trace_dbg_layer) {
acpi_dbg_layer = acpi_gbl_trace_dbg_layer;
}
}
(void)acpi_ut_release_mutex(ACPI_MTX_NAMESPACE);
exit:
if (enabled) {
ACPI_TRACE_POINT(ACPI_TRACE_AML_METHOD, TRUE,
obj_desc ? obj_desc->method.aml_start : NULL,
pathname);
}
if (pathname) {
ACPI_FREE(pathname);
}
}
void
acpi_ex_stop_trace_method(struct acpi_namespace_node *method_node,
union acpi_operand_object *obj_desc,
struct acpi_walk_state *walk_state)
{
acpi_status status;
char *pathname = NULL;
u8 enabled;
ACPI_FUNCTION_NAME(ex_stop_trace_method);
if (method_node) {
pathname = acpi_ns_get_normalized_pathname(method_node, TRUE);
}
status = acpi_ut_acquire_mutex(ACPI_MTX_NAMESPACE);
if (ACPI_FAILURE(status)) {
goto exit_path;
}
enabled = acpi_ex_interpreter_trace_enabled(NULL);
(void)acpi_ut_release_mutex(ACPI_MTX_NAMESPACE);
if (enabled) {
ACPI_TRACE_POINT(ACPI_TRACE_AML_METHOD, FALSE,
obj_desc ? obj_desc->method.aml_start : NULL,
pathname);
}
status = acpi_ut_acquire_mutex(ACPI_MTX_NAMESPACE);
if (ACPI_FAILURE(status)) {
goto exit_path;
}
if (acpi_gbl_trace_method_object == obj_desc) {
if (acpi_gbl_trace_flags & ACPI_TRACE_ONESHOT) {
acpi_gbl_trace_method_name = NULL;
}
acpi_dbg_level = acpi_gbl_original_dbg_level;
acpi_dbg_layer = acpi_gbl_original_dbg_layer;
acpi_gbl_trace_method_object = NULL;
}
(void)acpi_ut_release_mutex(ACPI_MTX_NAMESPACE);
exit_path:
if (pathname) {
ACPI_FREE(pathname);
}
}
void
acpi_ex_start_trace_opcode(union acpi_parse_object *op,
struct acpi_walk_state *walk_state)
{
ACPI_FUNCTION_NAME(ex_start_trace_opcode);
if (acpi_ex_interpreter_trace_enabled(NULL) &&
(acpi_gbl_trace_flags & ACPI_TRACE_OPCODE)) {
ACPI_TRACE_POINT(ACPI_TRACE_AML_OPCODE, TRUE,
op->common.aml, op->common.aml_op_name);
}
}
void
acpi_ex_stop_trace_opcode(union acpi_parse_object *op,
struct acpi_walk_state *walk_state)
{
ACPI_FUNCTION_NAME(ex_stop_trace_opcode);
if (acpi_ex_interpreter_trace_enabled(NULL) &&
(acpi_gbl_trace_flags & ACPI_TRACE_OPCODE)) {
ACPI_TRACE_POINT(ACPI_TRACE_AML_OPCODE, FALSE,
op->common.aml, op->common.aml_op_name);
}
}
