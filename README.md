# Bachelorarbeit
Code bei dem ich mich an Probleme herantaste

## man in the middle (mitm)
Hier geht es darum Pakete von einem Browser empfangen weiter an einen Socks Server zu schicken und die Antworten 
des Socks Servers zurück an den Browser zu senden.
Als Socks Server wird die Implementierung microsocks verwendet.
v0.1 und v0.2 haben nicht funktioniert. Und bei v0.3 hatte es geklappt bis ich ihn abgeändert 
habe um zu testen was passiert, wenn die Pakete out of order weiterleite werden.
(Der Browser streikt und beendet die Verbindung)
Damit es wieder funktioniert müssen in der Funktion connection_handeler in der for Schleife alle Blöcke nach
dem ersten /* recv message \*/  und /\* forward message */ weg.

## nacl
Hier habe ich getestet ob libsodium die Anforderungen erfüllen kann, um die alleinige Kryptobibliothek zu sein.

## microsocks
Eine Socksv5 Server Implementierung von GitHub, die bisher sehr gute Dienste leistet.
