Dim max
max = inputBox("���ֵ")
Randomize
if max = "" Then
    result = Rnd() * 100
Else
    result = Int(Rnd() * Int(max) + 1)
End If
msgbox(result)