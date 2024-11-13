<?php

// Set the content type to be HTML
header('Content-Type: text/html; charset=UTF-8');

// Start HTML output
echo "<!DOCTYPE html>";
echo "<html lang='en'>";
echo "<head>";
echo "<meta charset='UTF-8'>";
echo "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
echo "<title>Simple PHP Page</title>";
echo "<style>";
echo "body { font-family: Arial, sans-serif; background-color: #f4f4f4; color: #333; margin: 0; padding: 0; text-align: center; }";
echo "h1 { color: #007bff; }";
echo "p { font-size: 18px; }";
echo "</style>";
echo "</head>";
echo "<body>";

echo "<h1>Welcome to My Simple PHP Page</h1>";
echo "<p>This is a basic example of using PHP to display a simple web page.</p>";
echo "<p>PHP is a great server-side scripting language for building dynamic web pages!</p>";

echo "</body>";
echo "</html>";
?>
