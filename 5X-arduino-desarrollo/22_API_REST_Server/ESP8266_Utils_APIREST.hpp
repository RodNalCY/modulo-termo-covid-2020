int GetIdFromURL(AsyncWebServerRequest *request, String root)
{
  String string_id = request->url();
  string_id.replace(root, "");
  int id = string_id.toInt();
  return id;
}
