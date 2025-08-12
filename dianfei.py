import sys
import secrets

def main():
    """
    此脚本复制了 Java 版 dianfei 程序的功能。

    - 如果通过命令行提供了一个整数参数，脚本将输出一个从 0 到该整数（不包含）
      范围内的安全随机整数。
    - 如果没有提供命令行参数，脚本将输出一个 0.00 到 100.00 之间的
      安全随机浮点数，并格式化为两位小数。
    """
    try:
        # sys.argv 列表包含命令行参数，第一个元素 (sys.argv[0]) 是脚本名称。
        # 因此，我们检查列表长度是否大于 1 来判断是否有实际参数。
        if len(sys.argv) > 1:
            # 将第一个参数转换为整数作为上限。
            upper_bound = int(sys.argv[1])
            
            # secrets.randbelow(n) 生成一个范围在 [0, n) 内的安全随机整数，
            # 这与 Java 的 SecureRandom.nextInt(n) 功能相同。
            random_integer = secrets.randbelow(upper_bound)
            print(random_integer)
        
        else:
            # 如果没有提供参数。
            # secrets 库没有直接生成浮点数的函数，标准做法是生成一个较大范围的
            # 整数然后进行缩放，以确保密码学安全。
            # 为了得到两位小数的精度，我们生成一个 0 到 10000 之间的整数，然后除以 100。
            # 这会产生一个 0.00 到 100.00 之间的浮点数。
            random_value = secrets.randbelow(10001) / 100.0
            
            # 使用 f-string 将数字格式化为两位小数，
            # 这等同于 Java 的 DecimalFormat("#0.00")。
            print(f'{random_value:.2f}')

    except Exception:
        # 原 Java 程序会捕获所有异常并静默处理（不做任何事）。
        # Python 中的 'pass' 语句可以实现同样的效果。
        pass

if __name__ == "__main__":
    main()
