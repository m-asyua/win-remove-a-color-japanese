# win-remove-a-color-japanese

ウィンドウ内の特定の色を透明にするツールです．

<img src="https://user-images.githubusercontent.com/83494645/213596654-1a9d0539-767e-4823-a73b-b17194575e28.gif" width="400">


<img src="https://user-images.githubusercontent.com/83494645/213597037-41d9c76f-70e5-406a-8952-e61549be74bb.png" width="400">


・対象ソフト（ブラウザ等）によっては，ハードウェアアクセラレータ（ハードウェアアクセラレーション）の機能をオフにしておく必要があります．

・設定は保存できます．
本ツールを起動したときに，保存した設定にあるものと同じウィンドウ名のものがあれば，それを透明にすることもできます．

・WINAPI(SetLayeredWindowAttributes, LWA_COLORKEY)を用いています．
